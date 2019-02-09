#ifndef MAZE_H
#define MAZE_H

#include <cstddef>

namespace mzr
{
    class Cell
    {
        public: 
            /*! The cell is encoded according to the following format:
             *  |*******b| Top wall status (1 = up, 0 = down). 
             *  |******b*| Right wall status (1 = up, 0 = down). 
             *  |*****b**| Bottom wall status (1 = up, 0 = down). 
             *  |****b***| Left wall status (1 = up, 0 = down). 
             *  |***b****| Visited cell status (1 = visited, 0 = untested). 
             *  |**b*****| Part of path status (1 = path, 0 = discarted). 
             *  |*b******| Visited by builder (BUILT).
             *  |b*******| Not used. 
             */
            typedef unsigned char cell_t;
            /*! Cell constructor
             * the data needs to be filled by the user side.
             */
            Cell(cell_t data = 255) : m_data(data) {};
            /// set the bit at pos (starting from 0 to the highest valuable bit) to 1 using or operator
            void set_bit(unsigned char pos);
            /// set the bit at pos (starting from 0 to the highest valuable bit) to 0 using and operator
            void unset_bit(unsigned char pos);
            /// get the bit at pos (starting from 0 to the highest valuable bit) using and operator
            bool get_bit(unsigned char pos);
            void set_data(cell_t data);
            cell_t get_data();

        private:
            cell_t m_data;
    };
    class Maze
    {
        public:
            static const unsigned char VISITED = 4;
            static const unsigned char PATH = 5;
            static const unsigned char BUILT = 6;
            typedef enum
            {
                TopWall = 0,
                RightWall,
                BottomWall,
                LeftWall
            }side;
            Maze(size_t width, size_t height)
                : m_width(width), m_height(height),
                  m_cells(new Cell [ width * height])
            {
                size_t cells_len = m_width * m_height;
                Cell* current_cell = m_cells;
                for(size_t i = 0; i < cells_len; ++i)
                {
                    current_cell->set_data(0b00001111);
                    current_cell++;
                }
            }
            void knock_down(size_t x, size_t y , side s);
            Cell& get_cell(size_t x, size_t y) const;
            Cell& get_cell(size_t index) const;
            size_t get_height(){return m_height;}
            size_t get_width(){return m_width;}
            
        private:
            size_t m_width;
            size_t m_height;
            Cell* m_cells;
    };
}

#endif // MAZE_H
