#ifndef BUILDER_H
#define BUILDER_H

#include "render.h"
#include "maze.h"
#include <random>
#include <stack>

namespace mzr
{
    class Builder
    {
        public:
            Builder(Maze* mz) : 
                m_mz(mz), m_finished(false) {}
            void initalize();
            void next_step();
            bool is_finished();
            size_t get_step_number();

        private:

            Maze* m_mz;
            size_t m_cells_length;
            bool m_finished;
            std::mt19937 m_rng;

            std::stack<size_t> m_stack;
            size_t m_current_cell_index;
            size_t m_step_counter;
    };
}

#endif // BUILDER_H
