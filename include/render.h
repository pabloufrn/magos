#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <string>
#include "maze.h"
#include "canvas.h"
#include "common.h"
#include "color_config.h"

namespace mzr
{
    class Render
    {
        public:
            /*! Render default constructor.
             * @param mz Pointer to maze will be render.
             * @param width width of the canvas area.
             * @param height height of the canvas area.
             */
            Render(Maze* mz, size_t width, size_t height, size_t margin); 
            /// draw maze to the png file named by out_file param
            void draw(std::string out_file);
            /// initalize canvas
            void update_canvas();
            /// initalize color config
            void initialize_colors(ColorConfig* cfg);


        private:

            canvas::Canvas m_canvas;
            Maze* m_maze;
            size_t m_cell_width;
            size_t m_cell_height;
            size_t m_vertical_border;
            size_t m_horizontal_border;
            float m_cell_point_border_hint = 5;
            size_t m_min_border_size = 50;
            ColorConfig* m_config;
    };
}

#endif // RENDER_H
