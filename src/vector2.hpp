

struct Vector2
{
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x_val, float y_val) : x(x_val), y(y_val) {}

    float x = 0.0f;
    float y = 0.0f;
    
    Vector2 operator+(const Vector2& other) const {
        return Vector2{x + other.x, y + other.y};
    }
     Vector2 operator-(const Vector2& other) const {
        return Vector2{x - other.x, y - other.y};
    }
     Vector2 operator/(float number) const {
        return Vector2{x/number, y/number};
    }

     Vector2 operator*(float number) const {
        return Vector2{x*number, y*number};
    }

};



struct Vector2u_int16//OPTIMISE MEMORY
{
    Vector2u_int16() : x(0), y(0) {}
    Vector2u_int16(uint16_t x_val, uint16_t y_val) : x(x_val), y(y_val) {}
     Vector2u_int16(uint16_t val) : x(val), y(val) {}

    uint16_t x = 0;
    uint16_t y = 0;
    
    // Vector2u_int16 operator+(const Vector2u_int16& other) const {
    //     return Vector2u_int16{x + other.x, y + other.y};
    // }

    //   Vector2u_int16 operator+(const int number) const {
    //     return Vector2u_int16{x + number, y + number};
    // }
    //  Vector2u_int16 operator-(const Vector2u_int16& other) const {
    //     return Vector2u_int16{x - other.x, y - other.y};
    // }

};

enum ObstacleType {
    BLOCK,
    LINE
};


struct Obstacle
{
    Obstacle() : position(), real(false) {}
    Obstacle(float x_val, float y_val) : position(x_val, y_val), real(true) {}
     Obstacle(float x_val, float y_val,uint8_t hole_size) : position(x_val, y_val), real(true), hole_y_size(hole_size), type(LINE) {}

    Vector2 position = Vector2();
    bool real = false;
    ObstacleType type = BLOCK;

    //Line
    uint8_t hole_y_size = 0;
   

};

struct Particle {
     Particle() : position(), real(false), size(0) {}
    Particle(uint8_t size_val, float x_val, float y_val) : position(x_val, y_val), size(size_val), real(true) {}


    //WARNING : Particles position are not relative to the player, ther a re just on screen position
    Vector2 position = Vector2();
    uint8_t size = 0; //0 - 255 eq to 0 - 6 : So 255/40 = 6 approximately
    bool real = false;


};
