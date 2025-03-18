#pragma once

#include <functional>
#include <string>
#include <vector>

#include "timer.h"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define NONE 4

#define WIDTH 15
#define HEIGHT 15

#define TIMER_EVENT_START 0
#define TIMER_EVENT_STOP 1

namespace usnake {
    typedef struct {
        int x;
        int y;
        int attributes;
    } ATTRIBUTED_POINT;

    typedef ATTRIBUTED_POINT SNAKE_CELL;

    class Snake {
    private:
        std::vector<SNAKE_CELL> snake_cells;  
        ATTRIBUTED_POINT ap = { 0, 0 };

        //int pendingDirection = 0;
        bool directionPending = false;

        bool _isPaused = false;
        bool _gameover = false;
        int _score = 0;
        int direction = 0;

        Timer *timer;

        void init_cells();
        bool point_over_snake(int x, int y);
        void set_food_pos();
        void handle_food(SNAKE_CELL s);

    public:
        class Iterator {
        private:
            std::vector<SNAKE_CELL>::iterator it;
        public:
            Iterator(std::vector<SNAKE_CELL>::iterator itr) : it(itr) {}

            SNAKE_CELL& operator*() { return *it; }
            SNAKE_CELL* operator->() { return &(*it); }

            Iterator& operator++() { ++it; return *this; }
            Iterator operator++(int) { Iterator temp = *this; ++it; return temp; }

            bool operator==(const Iterator& other) const { return it == other.it; }
            bool operator!=(const Iterator& other) const { return it != other.it; }
        };

        int pendingDirection = 0;

        Iterator begin() { return Iterator(snake_cells.begin()); }
        Iterator end() { return Iterator(snake_cells.end()); }

        void new_game();
        void pause_game();
        void resume_game();
        void cycle();
        void set_direction(int dir);
        bool isPaused() const;
        bool gameover() const;
        int score() const;
        ATTRIBUTED_POINT food() const;
        Snake(Timer *timer);
        Snake();
    };
};