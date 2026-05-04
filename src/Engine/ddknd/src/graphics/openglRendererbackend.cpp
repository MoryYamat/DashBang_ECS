#include "graphics/gfx_type.h"
#include "graphics/renderer.h"

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
        spdlog::info("VS first bytes: {:02} {:02} {:02}", static_cast<unsigned char>(src[0]),
                     static_cast<unsigned char>(src[1]), static_cast<unsigned char>(src[2]));

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
        }

        types::GPUID<tag::ShaderProgramGPUTag> CreateShaderProgram(std::string_view vs_source,
                                                                   std::string_view fs_source) override
        {
            GLuint vs = compile_shader(GL_VERTEX_SHADER, vs_source);
            if (!vs)
                return {};

            GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fs_source);
            if (!fs)
            {
                glDeleteShader(vs);
                return {};
            }

            GLuint prog = link_program(vs, fs);
            glDeleteShader(vs);
            glDeleteShader(fs);
            if (!prog)
                return {};

            // 登録して発行
            const auto id_val = static_cast<std::uint32_t>(programs_.size());
            programs_.push_back(prog);

            return types::GPUID<tag::ShaderProgramGPUTag>(id_val);
        }

        void DestroyShaderProgram(types::GPUID<tag::ShaderProgramGPUTag> id) override
        {
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
            // test
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            // test

            const GLuint prog = get_program(id);
            glUseProgram(prog);

            glEnable(GL_DEPTH_TEST);
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
            const auto& prim = prims_[static_cast<std::size_t>(id.Value())];
            glBindVertexArray(prim.vao);
        }

        void DrawIndexed(std::uint32_t indexCount) override
        {
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        }

        GPUID<PrimitiveTag> CreateOrGetPrimitive(const ImportPrimitive& import, const PrimitiveKey& key) override
        {
            if (const auto it = primitiveCache_.find(key); it != primitiveCache_.end())
                return it->second;

            const auto id = GPUID<PrimitiveTag>(static_cast<std::uint32_t>(prims_.size()));

            buildPrimitiveGPUResource(import);
            primitiveCache_.emplace(key, id);

            return id;
        }

        void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Mat4f& m) override
        {
            const auto& prog = programs_[static_cast<std::size_t>(shader.Value())];

            GLint loc = glGetUniformLocation(prog, name);
            if (loc < 0)
                return;

            glUniformMatrix4fv(loc, 1,
                               GL_TRUE, // raw-major
                               m.Data()  // float*
            );
        }

      private:
        std::vector<GLuint> programs_;

        GLuint get_program(types::GPUID<tag::ShaderProgramGPUTag> id) const noexcept
        {
            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= programs_.size())
                return 0;
            return programs_[idx];
        }

        void buildPrimitiveGPUResource(const ImportPrimitive& import);

        struct GLPrimitive
        {
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ebo = 0;
        };
        std::vector<GLPrimitive> prims_;

        std::unordered_map<PrimitiveKey, GPUID<PrimitiveTag>, PrimitiveKeyHash> primitiveCache_;
    };

    // builder
    void OpenGLRendererBackend::buildPrimitiveGPUResource(const ImportPrimitive& import)
    {
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

        prims_.push_back(std::move(gl));
    }

    // =========================================== factory ===========================================
    std::unique_ptr<IRendererBackend> CreateOpenGLBackend(const OpenGLBackendDesc& desc)
    {
        return std::make_unique<OpenGLRendererBackend>();
    }
} // namespace ddknd::graphics