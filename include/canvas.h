#ifndef CANVAS_H
#define CANVAS_H

#include <cstring> // memset, memcpy
#include <iostream>
#include <stdexcept>

#include "common.h"

namespace canvas {

    //! Provides methods for drawing on an image.
    /*!
     * This is a drawing area on which we shall draw a maze representation.
     *
     * Here some information on the canvas properties:
     *
     * 1. The drawing area has the origin set to the top left corner, with
     * positive `X` axis going to the right and positive `Y` axis going to
     * the bottom of the canvas.
     * 2. Canvas supports basic operations to draw on vertical and horizontal
     * lines, as well as filled bordeless boxes.
     * 3. It is also possible to change line thikness to improve the final
     * appearance of the maze repreentation on the output image.
     *
     * This class returns to the client a pointer to the bytes (image) that
     * represents the pixels, which might be recorded by the client as an PPM
     * or PNG image file.
     */
    class Canvas
    {
        public:
            //=== Alias
            typedef unsigned char component_t; //!< Type of a color channel.

        public:

            //=== Special members
            /// Constructor
            /*! Creates an empty canvas of the requested size.
             * @param w_ The canvas width (in pixels).
             * @param h_ The canvas height (in pixels).
             */
            Canvas( size_t w=0, size_t h=0 )
                : m_width{w}, m_height{h},
                  m_pixels{  new component_t [ m_height * m_width * 3 ] },
                  m_line_thickness{1}
            { std::memset( m_pixels, 255, ( m_height * m_width * 3 )); }

            /// Destructor.
            virtual ~Canvas( void )
            {
                delete [] m_pixels;
            }

            //=== Special members
            /// Copy constructor.
            Canvas( const Canvas & );
            /// Assignment operator.
            Canvas & operator=( const Canvas & );

            //=== Members
            /// Clear the canvas with the background color.
            void clear( const Color& );
            /// Change the current line thickness.
            void thickness( unsigned short lt )
            { m_line_thickness = lt; }
            /// Get the current line thickness.
            unsigned short thickness( void ) const
            { return m_line_thickness; }
            /// Set the color of a pixel on the canvas.
            void pixel( coord_type x, coord_type y,  const Color& );
            /// Get the pixel color from the canvas.
            Color pixel( coord_type x, coord_type y ) const;

            //=== Attribute accessors members.
            /// Get the canvas width.
            size_t width( void ) const
            { return m_width; }
            /// Get the cancas height.
            size_t height( void ) const
            { return m_height; }
            /// Get the canvas pixels, as an array of `unsigned char`.
            component_t* buffer( void ) const
            { return m_pixels; }

            /// Draw a horizontal line with `length` pixels on the canvas, starting at a `p` location.
            /*
             * Example:
             *
             * ```
             * (p.x, p.y), thickness = 3, length = 15.
             *  |
             *  v
             *  . . . . . . . . . . . . . . .
             *  . . . . . . . . . . . . . . .
             *  . . . . . . . . . . . . . . .
             *  ```
             */
            void hline( coord_type x, coord_type y, size_t length, const Color& c );
            /// Draw a vertical line with `length` pixels on the canvas, starting at a `p` location.
            /*
             * Example:
             *
             * ```
             * (p.x, p.y), thickness = 3, length = 6.
             *  |
             *  v
             *  . . .
             *  . . .
             *  . . .
             *  . . .
             *  . . .
             *  . . .
             *  ```
             */
            void vline( coord_type x, coord_type y,  size_t length, const Color& c );
            /// Draw a filled box, with origin at `p` (the top-left corner).
            void box( coord_type x, coord_type y, size_t width, size_t height, const Color& c );

        private:
            size_t m_width; //!< The image width in pixel units.
            size_t m_height; //!< The image height in pixel unitsr
            component_t *m_pixels; //!< The pixels, stored as 3 RGB components.
            unsigned short m_line_thickness; //!< Current line thickness.
    };
} // namespace

#endif // CANVAS_H
