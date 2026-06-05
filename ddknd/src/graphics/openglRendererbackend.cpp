#include "ddknd/graphics/gfx_type.h"
#include "ddknd/graphics/renderer.h"

#include <cstddef>
#include <memory>
#include <span>
#include <string_view>
#include <vector>

#include "internal/graphics/model_importer/model_import_types.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

// utils
namespace
{
    GLuint compile_shader(GLenum stage, std::string_view src)
    {
        if (src.empty())
        {
            spdlog::error("Shader source is empty");
            return 0;
        }

        const char* stage_name = stage == GL_VERTEX_SHADER     ? "VERTEX"
                                 : stage == GL_FRAGMENT_SHADER ? "FRAGMENT"
                                                               : "OTHER";
        spdlog::info("{} shader source size: {}", stage_name, src.size());

        GLuint sh = glCreateShader(stage);
        const char* p = src.data();
        GLint len = static_cast<GLint>(src.size());
        glShaderSource(sh, 1, &p, &len);
        glCompileShader(sh);

        GLint ok = 0;
        glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            GLint log_len = 0;
            glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &log_len);

            std::string log;
            if (log_len > 0)
            {
                log.resize(static_cast<std::size_t>(log_len));
                GLsizei written = 0;
                glGetShaderInfoLog(sh, log_len, &written, log.data());
                if (written > 0)
                    log.resize(static_cast<size_t>(written));
            }

            const char* stage_name = (stage == GL_VERTEX_SHADER)     ? "VERTEX"
                                     : (stage == GL_FRAGMENT_SHADER) ? "FRAGMENT"
                                                                     : "OTHER";

            spdlog::error("Shader compile failed ({})\n{}", stage_name, log);

            glDeleteShader(sh);
            return 0;
        }

        return sh;
    }

    GLuint link_program(GLuint vs, GLuint fs)
    {
        GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);

        GLint ok = 0;
        glGetProgramiv(prog, GL_LINK_STATUS, &ok);
        if (ok != GL_TRUE)
        {
            GLint log_len = 0;
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &log_len);

            std::string log;
            if (log_len > 0)
            {
                log.resize(static_cast<size_t>(log_len));
                GLsizei written = 0;
                glGetProgramInfoLog(prog, log_len, &written, log.data());
                if (written > 0)
                    log.resize(static_cast<size_t>(written));
            }

            spdlog::error("Program link failed\n{}", log);

            glDeleteProgram(prog);
            return 0;
        }

        glDetachShader(prog, vs);
        glDetachShader(prog, fs);

        return prog;
    }
} // namespace

namespace ddknd::graphics
{
    class OpenGLRendererBackend final : public IRendererBackend
    {
        template <typename Tag>
        using GPUID = ::ddknd::graphics::types::GPUID<Tag>;

        // internal
        using ImportPrimitive = ::ddknd::graphics::internal::types::ImportPrimitive;
        using Vertex = ::ddknd::graphics::internal::types::Vertex;

        using PrimitiveTag = ::ddknd::graphics::tag::PrimitiveTag;
        using PrimitiveKey = ::ddknd::graphics::types::PrimitiveKey;
        using PrimitiveKeyHash = ::ddknd::graphics::types::PrimitiveKeyHash;

      public:
        OpenGLRendererBackend() = default;
        OpenGLRendererBackend(const OpenGLRendererBackend&) = delete;
        OpenGLRendererBackend& operator=(const OpenGLRendererBackend&) = delete;
        OpenGLRendererBackend(OpenGLRendererBackend&&) = delete;
        OpenGLRendererBackend& operator=(OpenGLRendererBackend&&) = delete;

        ~OpenGLRendererBackend()
        {

            for (GLuint& prog : programs_)
            {
                if (prog != 0)
                {
                    glDeleteProgram(prog);
                    prog = 0;
                }
            }

            for (auto& prim : prims_)
            {
                if (prim.vbo != 0)
                {
                    glDeleteBuffers(1, &prim.vbo);
                    prim.vbo = 0;
                }
                if (prim.vao != 0)
                {
                    glDeleteVertexArrays(1, &prim.vao);
                    prim.vao = 0;
                }
                if (prim.ebo != 0)
                {
                    glDeleteBuffers(1, &prim.ebo);
                    prim.ebo = 0;
                }
            }
            for (auto& tex : textures_)
            {
                if (tex != 0)
                {
                    glDeleteTextures(1, &tex);
                    tex = 0;
                }
            }
            for (auto& batch : screenQuadBatches_)
            {
                if (batch.vbo != 0)
                    glDeleteBuffers(1, &batch.vbo);

                if (batch.ebo != 0)
                    glDeleteBuffers(1, &batch.ebo);

                if (batch.vao != 0)
                    glDeleteVertexArrays(1, &batch.vao);
            }
            for (auto& batch : lineBatches_)
            {
                if (batch.vbo != 0)
                    glDeleteBuffers(1, &batch.vbo);

                if (batch.vao != 0)
                    glDeleteVertexArrays(1, &batch.vao);
            }
        }

        types::GPUID<tag::ShaderProgramGPUTag> CreateShaderProgram(std::string_view vs_source,
                                                                   std::string_view fs_source) override
        {
            GLuint vs = compile_shader(GL_VERTEX_SHADER, vs_source);
            if (!vs)
            {
                spdlog::error("CreateShaderProgram: vertex shader compile failed");
                return {};
            }

            GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fs_source);
            if (!fs)
            {
                spdlog::error("CreateShaderProgram: fragment shader compile failed");
                glDeleteShader(vs);
                return {};
            }

            GLuint prog = link_program(vs, fs);
            glDeleteShader(vs);
            glDeleteShader(fs);
            if (!prog)
            {

                spdlog::error("CreateShaderProgram: program link failed");
                return {};
            }

            // 登録して発行
            const auto id_val = static_cast<std::uint32_t>(programs_.size());
            programs_.push_back(prog);

            return types::GPUID<tag::ShaderProgramGPUTag>(id_val);
        }

        void DestroyShaderProgram(types::GPUID<tag::ShaderProgramGPUTag> id) override
        {
            if(!id.Is_valid())
                return;

            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= programs_.size())
                return;
            
            if (programs_[idx] != 0)
            {
                glDeleteProgram(programs_[idx]);
                programs_[idx] = 0;
            }
        }

        void UseShaderProgram(types::GPUID<tag::ShaderProgramGPUTag> id) override
        {
            if (!id.Is_valid())
            {
                spdlog::error("UseShaderProgram: invalid shader id");
                return;
            }

            const GLuint prog = try_get_program_handle(id);
            if (prog == 0)
            {
                spdlog::error("UseShaderProgram: shader program not found");
                return;
            }

            glUseProgram(prog);
        }

        types::GPUID<tag::PrimitiveTag> CreateMesh_Pos3(std::span<const float> xyz) override
        {
            // xyz.size()
            if (xyz.empty() || (xyz.size() % 3) != 0)
            {
                spdlog::error("OpenGLBackend::CreateMesh_Pos3: ");
                return types::GPUID<tag::PrimitiveTag>::Invalid();
            }

            GLPrimitive m{};

            glGenVertexArrays(1, &m.vao);
            glBindVertexArray(m.vao);

            glGenBuffers(1, &m.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(xyz.size() * sizeof(float)), xyz.data(),
                         GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            const std::uint32_t id = static_cast<std::uint32_t>(prims_.size());
            prims_.push_back(m);
            return types::GPUID<tag::PrimitiveTag>(id);
        }

        void DestroyMesh(types::GPUID<tag::PrimitiveTag> id) override
        {
            if (!id.Is_valid())
            {
                spdlog::error("OpenGLBackend::DestroyMesh:");
                return;
            }
            const auto idx = static_cast<std::uint32_t>(id.Value());
            if (idx >= prims_.size())
                return;
            auto& m = prims_[idx];
            if (m.vbo)
                glDeleteBuffers(1, &m.vbo);
            if (m.vao)
                glDeleteVertexArrays(1, &m.vao);
            m = GLPrimitive{};
        }

        void BindPrimitive(GPUID<PrimitiveTag> id) override
        {
            if (!id.Is_valid())
            {
                spdlog::error("BindPrimitive: invalid primitive id");
                return;
            }

            const auto idx = static_cast<std::size_t>(id.Value());
            if(idx >= prims_.size() || prims_[idx].vao == 0)
            {
                spdlog::error("BindPrimitive: primitive not found");
                return;
            }
            
            glBindVertexArray(prims_[idx].vao);
        }

        void DrawIndexed(std::uint32_t indexCount) override
        {
            if (indexCount == 0)
                return;

            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        }

        GPUID<PrimitiveTag> CreateOrGetPrimitive(const ImportPrimitive& import, const PrimitiveKey& key) override
        {
            if (const auto it = primitiveCache_.find(key); it != primitiveCache_.end())
                return it->second;

            const auto id = buildPrimitiveGPUResource(import);
            if (!id.Is_valid())
                return GPUID<PrimitiveTag>::Invalid();

            primitiveCache_.emplace(key, id);

            return id;
        }

        GPUID<tag::TextureTag> CreateTextureR8(int width, int height, std::span<const std::uint8_t> pixels) override
        {
            if (width <= 0 || height <= 0 || pixels.empty())
                return GPUID<tag::TextureTag>::Invalid();

            GLuint tex = 0;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.data());

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glBindTexture(GL_TEXTURE_2D, 0);

            const auto id = GPUID<tag::TextureTag>(static_cast<std::uint32_t>(textures_.size()));

            textures_.push_back(tex);
            return id;
        }

        void DestroyTexture(GPUID<tag::TextureTag> id) override
        {
            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= textures_.size())
                return;
            if (textures_[idx] != 0)
            {
                glDeleteTextures(1, &textures_[idx]);
                textures_[idx] = 0;
            }
        }
        void BindTexture2D(GPUID<tag::TextureTag> id, std::uint32_t slot) override
        {
            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= textures_.size())
                return;

            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, textures_[idx]);
        }

        void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Mat4f& m) override
        {
            if (name == nullptr)
            {
                spdlog::error("SetUniform(Mat4): uniform name is null");
                return;
            }

            const GLuint prog = try_get_program_handle(shader);
            if (prog == 0)
            {
                spdlog::error("SetUniform(Mat4): invalid shader");
                return;
            }

            GLint loc = glGetUniformLocation(prog, name);
            if (loc < 0)
                return;

            glUseProgram(prog);
            glUniformMatrix4fv(loc, 1,
                               GL_TRUE, // raw-major
                               m.Data() // float*
            );
        }

        void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Vec2f& v) override
        {
            if (name == nullptr)
            {
                spdlog::error("SetUniform(Vec2): uniform name is null");
                return;
            }

            const GLuint prog = try_get_program_handle(shader);
            if (prog == 0)
            {
                spdlog::error("SetUniform(Vec2): invalid shader");
                return;
            }

            const GLint loc = glGetUniformLocation(prog, name);
            if (loc < 0)
                return;

            glUseProgram(prog);
            glUniform2f(loc, v[0], v[1]);
        }
        void SetUniformMat4Array(GPUID<tag::ShaderProgramGPUTag> shader, const char* name,
                                 std::span<const math::Mat4f> matrices) override
        {
            if (name == nullptr)
            {
                spdlog::error("SetUniformMat4Array: uniform name is null");
                return;
            }

            if (matrices.empty())
                return;

            const GLuint prog = try_get_program_handle(shader);
            if (prog == 0)
            {
                spdlog::error("SetUniformMat4Array: invalid shader");
                return;
            }

            GLint loc = glGetUniformLocation(prog, name);
            if (loc < 0)
                return;

            std::vector<float> raw;
            raw.reserve(matrices.size() * 16);

            for (const auto& m : matrices)
            {
                const float* p = m.Data();
                raw.insert(raw.end(), p, p + 16);
            }

            glUseProgram(prog);
            glUniformMatrix4fv(loc, static_cast<GLsizei>(matrices.size()), GL_TRUE, raw.data());
        }

        GPUID<tag::ScreenQuadBatchTag> CreateScreenQuadBatch() override
        {
            GLScreenQuadBatch batch{};

            glGenVertexArrays(1, &batch.vao);
            glGenBuffers(1, &batch.vbo);
            glGenBuffers(1, &batch.ebo);

            glBindVertexArray(batch.vao);
            glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);

            constexpr GLsizei stride = sizeof(types::ScreenQuadVertex);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride,
                                  reinterpret_cast<void*>(offsetof(types::ScreenQuadVertex, pos)));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                                  reinterpret_cast<void*>(offsetof(types::ScreenQuadVertex, uv)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride,
                                  reinterpret_cast<void*>(offsetof(types::ScreenQuadVertex, color)));

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.ebo);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            const auto id = GPUID<tag::ScreenQuadBatchTag>(static_cast<std::uint32_t>(screenQuadBatches_.size()));

            screenQuadBatches_.push_back(batch);

            return id;
        }

        void UpdateScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag> id, std::span<const types::ScreenQuadVertex> vertices,
                                   std::span<const std::uint32_t> indices) override
        {
            if (!id.Is_valid())
                return;
            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= screenQuadBatches_.size())
                return;
            if (vertices.empty() || indices.empty())
                return;

            auto& batch = screenQuadBatches_[idx];

            glBindVertexArray(batch.vao);

            glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(),
                         GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size_bytes()), indices.data(),
                         GL_DYNAMIC_DRAW);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void DrawScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag> batchId, GPUID<tag::ShaderProgramGPUTag> shader,
                                 GPUID<tag::TextureTag> texture, std::uint32_t indexCount, int screenWidth,
                                 int screenHeight) override
        {
            if (!batchId.Is_valid() || !shader.Is_valid() || !texture.Is_valid())
                return;

            const auto batchIdx = static_cast<std::size_t>(batchId.Value());
            if (batchIdx >= screenQuadBatches_.size())
                return;

            const GLuint prog = try_get_program_handle(shader);
            if (prog == 0)
            {
                spdlog::error("DrawScreenQuadBatch: invalid shader");
                return;
            }

            const auto& batch = screenQuadBatches_[batchIdx];

            glDisable(GL_DEPTH_TEST);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glUseProgram(prog);

            glUniform2f(glGetUniformLocation(prog, "uScreenSize"), static_cast<float>(screenWidth),
                        static_cast<float>(screenHeight));

            glUniform1i(glGetUniformLocation(prog, "uTexture"), 0);

            BindTexture2D(texture, 0);

            glBindVertexArray(batch.vao);

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);

            // TODO: Move GL renderer States to RenderPass / PipelineState
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        }

        void DestroyScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag> id) override
        {
            if (!id.Is_valid())
                return;

            const auto idx = static_cast<std::size_t>(id.Value());

            if (idx >= screenQuadBatches_.size())
                return;

            auto& batch = screenQuadBatches_[idx];
            if (batch.vbo != 0)
            {
                glDeleteBuffers(1, &batch.vbo);
                batch.vbo = 0;
            }
            if (batch.ebo != 0)
            {
                glDeleteBuffers(1, &batch.ebo);
                batch.ebo = 0;
            }
            if (batch.vao != 0)
            {
                glDeleteVertexArrays(1, &batch.vao);
                batch.vao = 0;
            }
        }

        // ************ Drawing Line ************
        GPUID<tag::LineBatchTag> CreateLineBatch() override
        {
            GLLineBatch batch{};

            glGenVertexArrays(1, &batch.vao);
            glGenBuffers(1, &batch.vbo);

            glBindVertexArray(batch.vao);
            glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);

            constexpr GLsizei stride = sizeof(types::LineVertex);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                                  reinterpret_cast<void*>(offsetof(types::LineVertex, pos)));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride,
                                  reinterpret_cast<void*>(offsetof(types::LineVertex, color)));

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            const auto id = GPUID<tag::LineBatchTag>(static_cast<std::uint32_t>(lineBatches_.size()));

            lineBatches_.push_back(batch);
            return id;
        }
        void UpdateLineBatch(GPUID<tag::LineBatchTag> id, std::span<const types::LineVertex> vertices) override
        {
            if (!id.Is_valid())
                return;

            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= lineBatches_.size())
                return;

            if (vertices.empty())
                return;

            auto& batch = lineBatches_[idx];

            glBindVertexArray(batch.vao);

            glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);

            // ***** update vbo *****
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(),
                         GL_DYNAMIC_DRAW);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        void DrawLineBatch(GPUID<tag::LineBatchTag> id, GPUID<tag::ShaderProgramGPUTag> shader,
                           std::uint32_t vertexCount) override
        {
            if (!id.Is_valid() || !shader.Is_valid())
            {
                spdlog::error("DrawLineBatch: invalid id or shader");
                return;
            }

            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= lineBatches_.size())
            {
                spdlog::error("DrawLineBatch: invalid id");
                return;
            }

            if (vertexCount == 0)

                return;

            const auto& batch = lineBatches_[idx];

            const GLuint prog = try_get_program_handle(shader);
            if (prog == 0)
            {
                spdlog::error("DrawLineBatch: invalid shader");
                return;
            }

            glUseProgram(prog);

            glLineWidth(1.0f);

            glDisable(GL_DEPTH_TEST);

            glBindVertexArray(batch.vao);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertexCount));

            glBindVertexArray(0);
            glEnable(GL_DEPTH_TEST);
        }
        void DestroyLineBatch(GPUID<tag::LineBatchTag> id) override
        {
            if (!id.Is_valid())
                return;

            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= lineBatches_.size())
                return;

            auto& batch = lineBatches_[idx];

            if (batch.vbo != 0)
            {
                glDeleteBuffers(1, &batch.vbo);
                batch.vbo = 0;
            }

            if (batch.vao != 0)
            {
                glDeleteVertexArrays(1, &batch.vao);
                batch.vao = 0;
            }
        }

      private:
        std::vector<GLuint> programs_;

        GLuint try_get_program_handle(types::GPUID<tag::ShaderProgramGPUTag> id) const noexcept
        {
            if (!id.Is_valid())
                return 0;

            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= programs_.size())
                return 0;
            return programs_[idx];
        }

        GPUID<PrimitiveTag> buildPrimitiveGPUResource(const ImportPrimitive& import);

        struct GLPrimitive
        {
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ebo = 0;
        };
        std::vector<GLPrimitive> prims_;

        struct GLScreenQuadBatch
        {
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ebo = 0;
        };

        struct GLLineBatch
        {
            GLuint vao = 0;
            GLuint vbo = 0;
        };

        std::vector<GLuint> textures_;
        std::vector<GLScreenQuadBatch> screenQuadBatches_;
        std::vector<GLLineBatch> lineBatches_;
        std::unordered_map<PrimitiveKey, GPUID<PrimitiveTag>, PrimitiveKeyHash> primitiveCache_;
    };

    // builder
    ::ddknd::graphics::types::GPUID<::ddknd::graphics::tag::PrimitiveTag> OpenGLRendererBackend::
        buildPrimitiveGPUResource(const ImportPrimitive& import)
    {
        if (import.vertices.empty() || import.indices.empty())
            return GPUID<::ddknd::graphics::tag::PrimitiveTag>::Invalid();

        GLPrimitive gl;
        glGenVertexArrays(1, &gl.vao);
        glGenBuffers(1, &gl.vbo);
        glGenBuffers(1, &gl.ebo);

        // bind
        glBindVertexArray(gl.vao);

        const auto vertices_size = import.vertices.size() * sizeof(decltype(import.vertices)::value_type);
        const auto indices_size = import.indices.size() * sizeof(decltype(import.indices)::value_type);
        // setup vbo
        glBindBuffer(GL_ARRAY_BUFFER, gl.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_size, import.vertices.data(), GL_STATIC_DRAW);

        // setup ebo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, import.indices.data(), GL_STATIC_DRAW);

        using V = typename decltype(import.vertices)::value_type;
        static_assert(std::is_standard_layout_v<V>, "Vertex must be standard layout");
        static_assert(std::is_trivially_copyable_v<V>);
        GLsizei stride = sizeof(V);
        // setup vao
        // position (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);

        // normal (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, normal)));
        glEnableVertexAttribArray(1);

        // texcoords (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
        glEnableVertexAttribArray(2);

        // tangent (location = 3)
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, tangent)));
        glEnableVertexAttribArray(3);

        // joints (location = 4)
        glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, stride, reinterpret_cast<void*>(offsetof(Vertex, joints)));
        glEnableVertexAttribArray(4);

        // weights (location = 5)
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, weights)));
        glEnableVertexAttribArray(5);

        glBindVertexArray(0);

        const auto id = GPUID<PrimitiveTag>(static_cast<std::uint32_t>(prims_.size()));
        prims_.push_back(std::move(gl));
        return id;
    }

    // =========================================== factory ===========================================
    std::unique_ptr<IRendererBackend> CreateOpenGLBackend(const OpenGLBackendDesc& desc)
    {
        return std::make_unique<OpenGLRendererBackend>();
    }
} // namespace ddknd::graphics