#include "../include/canvas.h"

namespace canvas {

    /*!
     * Deep copy of the canvas.
     * @param clone The object we are copying from.
     */
    Canvas::Canvas( const Canvas & clone )
    {
        m_height = clone.m_height;
        m_width = clone.m_width;
        m_line_thickness = clone.m_line_thickness;
        // number of pixels from canvas
        size_t canvas_size = m_width * m_height;
        m_pixels = new component_t[canvas_size]; 
        component_t* pixelOriginRef = m_pixels; 
        component_t* pixelCloneRef = clone.m_pixels; 
        for(size_t i = 0; i < canvas_size; ++i)
        {
            *pixelOriginRef++ = *pixelCloneRef++; 
        }
    }


    /*!
     * @param source The object we are copying information from.
     * @return A reference to the `this` object.
     */
    Canvas& Canvas::operator=( const Canvas & source )
    {
        // number of pixels from canvas
        size_t canvas_size = source.m_width * source.m_height;
        if(m_height != source.m_height or m_width != source.m_width)
        {
            m_height = source.m_height;
            m_width = source.m_width;
            delete [] m_pixels;
            m_pixels = new component_t[canvas_size]; 
        }
        m_line_thickness = source.m_line_thickness;
        component_t* pixelOriginRef = m_pixels; 
        component_t* pixelCloneRef = source.m_pixels; 
        for(size_t i = 0; i < canvas_size; ++i)
        {
            *pixelOriginRef++ = *pixelCloneRef++; 
        }

        return *this;
    }

    void Canvas::clear( const Color& color )
    {
        // number of pixels from canvas
        size_t canvas_size = m_width * m_height;
        component_t* pixelRef = m_pixels; 
        for(size_t i = 0; i < canvas_size; ++i)
        {
            std::memcpy(pixelRef, color.channels, sizeof(Color::color_t)*3);
            pixelRef+=3;
        }
    }

    /*!
     * @throw `std::invalid_argument()` it the pixel coordinate is located outside the canvas.
     * @param p The 2D coordinate of the pixel we want to know the color of.
     * @return The pixel color.
     */
    Color Canvas::pixel( coord_type x, coord_type y ) const
    {
        component_t* pixelRef = &m_pixels[3*(y * m_width + x)]; 
        return Color(pixelRef[0], pixelRef[1], pixelRef[2]);
    }

    /*!
     * @note Nothing is done if the  pixel coordinate is located outside the canvas.
     * @param p The 2D coordinate of the pixel we want to change the color.
     * @param c The color.
     */
    void Canvas::pixel( coord_type x, coord_type y, const Color& c )
    {
        if(x < 0 or y < 0 or x > m_width or y > m_height)
            return;
        component_t* pixelRef = &m_pixels[3*(y * m_width + x)]; 
        std::memcpy(pixelRef, c.channels, sizeof(Color::color_t)*3);
    }

    /*!
     * Draws on the canvas a horizontal line.
     * @param p The 2D coordinate of the initial pixel of the line.
     * @param length The horizontal length of the line in pixels.
     */
    void Canvas::hline( coord_type x, coord_type y, size_t length, const Color& color )
    {
        if(y < 0 or y > m_height)
            return;
        if(x < 0)
        {
            size_t diff = -x;
            if(diff >= length)
                return;
            length -= diff;
            x = 0;
        }
        if(x + length > m_width)
        {
            size_t diff = x + length - m_width;
            if(diff >= length)
                return;
            length -= diff;
        }
        
        component_t* pixelRef = &m_pixels[3*(y * m_width + x)]; 
        for(size_t t = 0; t < m_line_thickness; ++t)
        {
            for(size_t i = 0; i < length; ++i)
            {
                std::memcpy(pixelRef, color.channels, sizeof(Color::color_t)*3);
                pixelRef+=3;
            }
            pixelRef += 3*(m_width - length);
        }
    }

    /*!
     * Draws on the canvas a vertical line.
     * @param p The 2D coordinate of the initial pixel of the line.
     * @param length The vertical length of the line in pixels.
     */
    void Canvas::vline( coord_type x, coord_type y, size_t length, const Color& color )
    {
        if(x < 0 or x > m_width)
            return;
        if(y < 0)
        {
            size_t diff = -y;
            if(diff >= length)
                return;
            length -= diff;
            y = 0;
        }
        if(y + length > m_height)
        {
            size_t diff = x + length - m_height;
            if(diff >= length)
                return;
            length -= diff;
        }
        component_t* pixelRef = &m_pixels[3*(y * m_width + x)]; 
        for(size_t i = 0; i < length; ++i)
        {
            for(size_t t = 0; t < m_line_thickness; ++t)
            {
                std::memcpy(pixelRef+3*t, color.channels, sizeof(Color::color_t)*3);
            }
            pixelRef += m_width*3;
        }
    }

    /*!
     * Draws on the canvas a filled box. The origin of the box is the top-left corner.
     * The box is compose of horizontal lines, drawn top to bottom.
     * @param p The 2D coordinate of the initial pixel of the box (top-left corner).
     * @param width The box's width in pixels.
     * @param height The box's heigth in pixels.
     */
    void Canvas::box( coord_type x, coord_type y, size_t width, size_t height , const Color& color)
    {
        component_t* pixelRef = &m_pixels[3*(y * m_width + x)]; 
        for(size_t h = 0; h < height; ++h)
        {
            for(size_t w = 0; w < width; ++w)
            {
                std::memcpy(pixelRef, color.channels, sizeof(Color::color_t)*3);
                pixelRef += 3;
            }
            pixelRef += 3*(m_width - width);
        }
    }
}

//================================[ canvas.cpp ]================================//
