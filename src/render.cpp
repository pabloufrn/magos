#include "../include/render.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

using namespace mzr;

Render::Render(Maze* mz, size_t width, size_t height, size_t margin) :
    m_canvas(width, height), m_maze(mz), m_min_border_size(margin), m_config(nullptr) 
{
    auto mz_width = m_maze->get_width();
    auto mz_height = m_maze->get_height();

    m_cell_width =  (width  + mz_width - 1  - m_min_border_size*2) / mz_width;
    m_cell_height = (height + mz_height - 1 - m_min_border_size*2) / mz_height;

    size_t offset_x = width  -   (m_cell_width   - 1) *  (mz_width)  - 1 - m_min_border_size*2;
    size_t offset_y = height -   (m_cell_height  - 1) *  (mz_height) - 1 - m_min_border_size*2;

    if(m_cell_width < 3 or m_cell_height < 3)
    {
        throw std::runtime_error("Invalid image size, the minimium size for this "
            "image is " + std::to_string(2 * mz_width + 1) + " x " + std::to_string(2 * mz_height + 1));
    }

    if(m_cell_width < m_cell_height)
    {
        m_cell_height = m_cell_width;
        m_horizontal_border = m_min_border_size + offset_x/2;
        m_vertical_border = (height - (m_cell_height - 1) * mz_height ) / 2;
    }
    else if(m_cell_width > m_cell_height)
    {
        m_cell_width = m_cell_height;
        m_vertical_border = m_min_border_size + offset_y/2;
        m_horizontal_border = (width - (m_cell_width - 1) * mz_width )/2;
    }
    else
    {
        m_vertical_border = m_min_border_size;
        m_horizontal_border = m_min_border_size;
    }

    this->update_canvas();
}

void Render::draw(std::string out_file)
{
    stbi_write_png_compression_level = 0;   // defaults to 8; set to higher for more compression
    auto width = m_canvas.width();
    auto height = m_canvas.height();
    auto pixels = m_canvas.buffer();
    auto result = stbi_write_png( out_file.c_str(),       // file name
                    width, height,  // image dimensions
                    3,              // # of channels per pixel
                    pixels,         // the pixels
                    width*3);       // length of a row (in bytes), see above.
    if(result == 0)
    {
            throw std::runtime_error("[Error] MagosGame failed to save png.");
    }
}

void Render::update_canvas()
{
    if(m_config == nullptr)
    {
        m_config = new ColorConfig();
        m_config->load_default();
    }
    m_canvas.clear(m_config->at("background-color"));
    // get all cell and draw
    size_t mz_height = m_maze->get_height();
    size_t mz_width = m_maze->get_width();
    size_t point_border_left = (m_cell_width - 2) / m_cell_point_border_hint;
    size_t point_border_top =  (m_cell_height - 2) / m_cell_point_border_hint;
    size_t point_width = m_cell_width - point_border_left*2 - 2;
    size_t point_height = m_cell_height - point_border_top*2 - 2;

    for(size_t y = 0; y < mz_height; ++y)
    {
        for(size_t x = 0; x < mz_width; ++x)
        {
            Cell c = m_maze->get_cell(x, y);
            if(y == 0 and c.get_bit(Maze::side::TopWall))
            {

                size_t coord_x = m_horizontal_border + x*(m_cell_width-1);
                size_t coord_y = m_vertical_border;
                size_t length = m_cell_width;
                m_canvas.hline(coord_x, coord_y, length, m_config->at("wall-color"));
            }
            if(c.get_bit(Maze::side::BottomWall))
            {
                size_t coord_x = m_horizontal_border + x*(m_cell_width-1);
                size_t coord_y = m_vertical_border + (y+1)*(m_cell_height-1);
                size_t length = m_cell_width;
                m_canvas.hline(coord_x, coord_y, length, m_config->at("wall-color"));
            }
            if(x == 0 and c.get_bit(Maze::side::LeftWall))
            {
                size_t coord_x = m_horizontal_border;
                size_t coord_y = m_vertical_border + y*(m_cell_height-1);
                size_t length = m_cell_height;
                m_canvas.vline(coord_x, coord_y, length, m_config->at("wall-color"));
            }
            if(c.get_bit(Maze::side::RightWall))
            {
                size_t coord_x = m_horizontal_border + (x+1)*(m_cell_width-1);
                size_t coord_y = m_vertical_border + y*(m_cell_height-1);
                size_t length = m_cell_height;
                m_canvas.vline(coord_x, coord_y, length, m_config->at("wall-color"));
            }
            // draw path points
            if(c.get_bit(Maze::VISITED))
            {
                size_t coord_x = m_horizontal_border + x*(m_cell_width-1) + point_border_left + 1;
                size_t coord_y = m_vertical_border + y*(m_cell_height-1) + point_border_top + 1;
                canvas::Color color;

                if(not (x == 0 and y == 0) and not (x == mz_width-1 and y == mz_height-1))
                {
                    if(c.get_bit(Maze::PATH))
                    {
                        color = m_config->at("path_point-color");
                    }
                    else
                    {
                        color = m_config->at("discarted_point-color");
                    }
                    m_canvas.box(coord_x, coord_y,
                            point_width, point_height, color);
                }
                
                // connect it to neighbors
                // right neighbor
                if(x < mz_width and not c.get_bit(Maze::side::RightWall))
                {
                    if(m_maze->get_cell(x + 1, y).get_bit(Maze::VISITED))
                    {
                        if(m_maze->get_cell(x + 1, y).get_bit(Maze::PATH))
                            color = m_config->at("path_point-color");
                        else
                            color = m_config->at("discarted_point-color");

                        m_canvas.box(coord_x + point_width, coord_y, 
                            point_border_left * 2 + 1, point_height, color);  
                    }
                }
                // bottom neighbor
                if(y < mz_height and not c.get_bit(Maze::side::BottomWall))
                {
                    if(m_maze->get_cell(x, y + 1).get_bit(Maze::VISITED))
                    {
                        if(c.get_bit(Maze::PATH) and m_maze->get_cell(x, y + 1).get_bit(Maze::PATH))
                            color = m_config->at("path_point-color");
                        else
                            color = m_config->at("discarted_point-color");

                        m_canvas.box(coord_x, coord_y + point_height, 
                                point_width, point_border_top * 2 + 1, color);  
                    }
                }
            }
        }
    }
    // Draw entrance and exit

    size_t coord_x = point_border_left + m_horizontal_border + 1;
    size_t coord_y = point_border_top + m_vertical_border + 1;
    canvas::Color color = m_config->at("entrance_point-color");

    m_canvas.box(coord_x, coord_y,
            point_width, point_height, color);

    coord_x = point_border_left + m_horizontal_border + (mz_width-1)*(m_cell_width-1) + 1;
    coord_y = point_border_top + m_vertical_border + (mz_height-1)*(m_cell_height-1) + 1;
    color = m_config->at("exit_point-color");

    m_canvas.box(coord_x, coord_y,
            point_width, point_height, color);
}
void Render::initialize_colors(ColorConfig* cfg)
{
    if(!cfg->is_loaded())
        throw std::runtime_error("colors not loaded");
    m_config = cfg;
}
