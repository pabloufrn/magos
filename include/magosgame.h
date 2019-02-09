#include <string>
#include <iostream>
#include "maze.h"
#include "render.h"
#include "builder.h"
#include "solver.h"
#include "color_config.h"

namespace mzr
{
    class MagosGame
    {
        public:
            enum class game_state_e
            {
                OK,
                ERROR
            };

            struct Result
            {
                MagosGame::game_state_e type;
                std::string msg;
            };

            enum class game_state
            {
                BUILDING,
                SOLVING,
                FINISHED
            };

            MagosGame() : m_mz(nullptr), m_render(nullptr), m_colors(nullptr) {}
            ~MagosGame();

            Result initialize(size_t rows, size_t cols, size_t width, size_t height, size_t margin = 50);
            void welcome();
            void process_events();
            void update();
            void render();
            bool game_over();
            void set_building_output_format(std::string format);
            void set_solving_output_format(std::string format);
            void set_solver_output_path(std::string path);
            void set_builder_output_path(std::string format);
            template < typename T, typename ConvertStringFunction>
            void load_colors(std::map<std::string, T>& map, const ConvertStringFunction& csf)
            {
                m_colors = new ColorConfig();
                m_colors->load_from_map(map, csf);
            }

        private:
            Maze* m_mz;
            Render* m_render;
            Builder* m_builder;
            Solver* m_solver;
            game_state m_state;
            bool m_wait_user;
            bool m_state_trasition;
            std::string m_solver_path;
            std::string m_builder_path;
            std::string m_building_prefix;
            std::string m_building_suffix;
            std::string m_solving_prefix;
            std::string m_solving_suffix;
            ColorConfig* m_colors;
    };
}
