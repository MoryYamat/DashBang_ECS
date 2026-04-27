#include "graphics/renderer.h"

#include <memory>
#include <string_view>
#include <vector>
#include <span>

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

            for (auto& mesh : meshes_)
            {
                if (mesh.vbo != 0)
                {
                    glDeleteBuffers(1, &mesh.vbo);
                    mesh.vbo = 0;
                }
                if (mesh.vao != 0)
                {
                    glDeleteVertexArrays(1, &mesh.vao);
                    mesh.vao = 0;
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
            const GLuint prog = get_program(id);
            glUseProgram(prog);
        }

        types::GPUID<tag::MeshGPUTag> CreateMesh_Pos3(std::span<const float> xyz) override
        {
            // xyz.size()
            if (xyz.empty() || (xyz.size() % 3) != 0)
            {
                spdlog::error("OpenGLBackend::CreateMesh_Pos3: ");
                return types::GPUID<tag::MeshGPUTag>::Invalid();
            }

            GLMesh m{};

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

            const std::uint32_t id = static_cast<std::uint32_t>(meshes_.size());
            meshes_.push_back(m);
            return types::GPUID<tag::MeshGPUTag>(id);
        }

        void DestroyMesh(types::GPUID<tag::MeshGPUTag> id) override
        {
            if (!id.Is_valid())
            {
                spdlog::error("OpenGLBackend::DestroyMesh:");
                return;
            }
            const auto idx = static_cast<std::uint32_t>(id.Value());
            if (idx >= meshes_.size())
                return;
            auto& m = meshes_[idx];
            if (m.vbo)
                glDeleteBuffers(1, &m.vbo);
            if (m.vao)
                glDeleteVertexArrays(1, &m.vao);
            m = GLMesh{};
        }

        void DrawArraysTriangles(std::uint32_t count) override
        {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count));
        }

        void BindMesh(types::GPUID<tag::MeshGPUTag> id) override
        {
            if (!id.Is_valid())
            {
                spdlog::error("OpenGLBackend::BindMesh: ");
                glBindVertexArray(0);
                return;
            }
            const auto idx = static_cast<std::size_t>(id.Value());
            if (idx >= meshes_.size())
            {
                glBindVertexArray(0);
                spdlog::error("OpenGLBackend::BindMesh: ");
                return;
            }

            glBindVertexArray(meshes_[idx].vao);
        }

      private:
        std::vector<GLuint> programs_;

		GLuint get_program(types::GPUID<tag::ShaderProgramGPUTag> id) const noexcept
		{
			const auto idx = static_cast<std::size_t>(id.Value());
			if (idx >= programs_.size())  return 0;
			return programs_[idx];
		}

        struct GLMesh
        {
            GLuint vao = 0;
            GLuint vbo = 0;
        };
        std::vector<GLMesh> meshes_;
    };

    std::unique_ptr<IRendererBackend>
	CreateOpenGLBackend(const OpenGLBackendDesc& desc)
	{

		return std::make_unique<OpenGLRendererBackend>();
	}
} // namespace ddknd::graphics