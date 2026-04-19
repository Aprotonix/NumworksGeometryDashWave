#include "display.h"
#include "eadkpp.h"
#include "palette.h"
#include "vector2.hpp"
#include "color.hpp"
#include "storage.h"
#include "string.h"
#include "texture.hpp"
#include "eadk.h"




#define WIDTH 320
#define HEIGHT 240
#define TPS 40
#define SPT 1/TPS
#define TrailLength 17
#define ObstacleMax 20
#define ParticleMax 20

#define STRING false
#define DEBUG false

// #define difficulty_1_color 0x3381ff
// #define difficulty_2_color 0x7708ff
// #define difficulty_3_color 0x8f0000
//#define MaxInt16Size 65535


enum Statue {
    MENU,
    LEVEL,
    DEATH,
    PAUSE
};

enum LevelType {
    RANDOM,
    DIAGONAL
};


class Game {
public:

    const uint64_t MSPT = 31;//ms per tick

    Game();

    Statue current_statue = MENU;

    LevelType old_level_type= RANDOM;
    LevelType current_level_type= RANDOM;
    LevelType next_level_type= DIAGONAL;

    // int old_level_type_x = 0;
    uint16_t current_level_type_x_started = 320;
    uint16_t next_level_type_x = 1000;

    void goToDeadStatue();

    bool quit = false;//To main detecting quit
    void quitGame();

    uint16_t score = 0;
    uint16_t high_score = 0;
    uint16_t difficulty = 0;

    bool has_jumped = false;
    uint16_t jump_counter = 0;
   // uint8_t jump_speed = 0;

    uint8_t reset_counter = 0;//debug

// Diagonale blanche 10x10, RGB565 correct, buffer plat



    // static_assert(sizeof(player_fram_up) / sizeof(EADK::Color) == 400,
    //           "player_fram_up n'a pas exactement 400 pixels !");


   // const int deltaTick16 = static_cast<int>(65535 / TPS)

    Vector2 camera_position;
    const Vector2 camera_offset = {60.0f, 0.0f};
    
    uint8_t player_frame = 0; //0 Down, 1 Half Down, 2 Middle, 3 Half Up, 4 Up
    //uint8_t frame_tick_counter = 0; //For 2 ticks per frame (Slow down animation)
    bool go_to_up_fram = false;
    const Vector2u_int16 player_size = Vector2u_int16{10};
   
    Vector2 player_position = {0.0f, 110.0f};
    Vector2 player_velocity = {320.0f/TPS, 320.0f/TPS};
    const float player_real_vy = 320.0f/TPS;
    bool player_dead = false;


    float adding_trail_size = 15.f/TPS;
    Vector2 player_trail[TrailLength];
    float trail_size = 6; //CAN CHANGE
    const Vector2u_int16 trail_inside_size = Vector2u_int16{6};


   // const float ob_alpha_adding = 3.f/TPS;
    const float x_end_spawn_animation = 270;
    const float x_start_disspawn_animation = 30;
    const Vector2u_int16 ob_size = Vector2u_int16{20};
    const Vector2u_int16 ob_inside_size = Vector2u_int16{18};//16 or 18
    const int ob_line_x_size = ob_size.x + 4;
    Obstacle obstacles[ObstacleMax];
    Vector2 next_obstacle_position = {320, 200};
    
    
    //DIAG MODE
    int diagonal_direction = 1;
    const int diagonal_error_correction = 5;
    bool ob_corruption = false;
    uint8_t spacing = 100;
    uint8_t line_spawn_max_y = 170;
    uint8_t line_spawn_min_y = 100;
    uint8_t straight_line_duration = 0; //0 = no straight line, diagonal only, 1 = juste ended
    

    Particle particles[ParticleMax];
    int next_particle_player_x = 320;//WORLD POSITION
    uint8_t particles_speed_boost = 0;//X speed added, decrease each [particles_step_tick] ticks
    uint8_t particles_speed_boost_descrease_step_tick_counter = 0; 
    uint8_t particles_speed_boost_descrease_step_tick = 5;//Each [particles_step_tick] ticks
    float speed_boost_start_x_player = 0;//in tick before start speed boost
    //WTF this variable


    //COLORS
    int current_background =0x000000;
    float background_alpha = 0.1f;
    float delta_background_alpha = 0.03f/TPS;

    const Color background2 = Color(0, 0, 200);
    const Color background1 = Color(200, 0, 220);

    float obstacles_color_alpha = 0.1f;
    float delta_obstacles_color_alpha = 0.03f/TPS;

    const Color obstacles_color2 = Color(150, 150, 255);
    const Color obstacles_color1 = Color(255, 150, 255);

    const Color particle_color_initial = Color(255, 255, 255);//Color will be blended with background color, so it will be better if its color is close to the background color
    // const Color obstacles_color1 = Color(0, 0, 150);
    // const Color obstacles_color2 = Color(150, 0, 150);

    const uint32_t difficulty_1_color = 0x3381ff;
    const uint32_t difficulty_2_color = 0x7708ff;
    const uint32_t difficulty_3_color = 0x8f0000;

    const char * EASY_BEST_FILE = "hse.sav";
    const char * MEDIUM_BEST_FILE = "hsm.sav";
    const char * HARD_BEST_FILE = "hsh.sav";

    //MENU
    uint8_t start_difficulty_chosen = 1;
    
    //PAUSE
    void quitPauseMode();
    void goToPause();

    //LEVEL GESTION
    void startLevel();
    void startDiagonalLevel();
    void   startBlockRandomLevel();

    void update();
    void updatePlayer();
    void updateLevel();

    //PARTICLES
    void generateAllParticles();
    void updateParticles();
    void clearParticles();
    void particlesSpeedBoost(uint8_t speed, float x_player_coord, uint16_t descrease_step_tick);

    //TRAIL
    void updateTrail();
    void clearTrail();

    //OBSTACLES
    void addObstacle(float x, float y);
    void addObstacleLine(float x,float y, uint8_t hole_size);
    void updateObstacles();
    void clearObstacles();

     void handleInput();

    bool isPlayerShouldDie();
    void respawn();
    //void diedLoop();

    //RENDER
    void renderTexture(int x, int y, int sizex, int sizey, const EADK::Color* texture);
    void renderTextureWithVoid(int x, int y, int sizex, int sizey, const EADK::Color* texture);


    void drawRectSafe(int x, int y, int sizex, int sizey, int color);
    void drawLine(int x1, int y1, int x2, int y2, int step, int size, int timestep);

    void diagonalTransition();
    void diagonalTransition2();
    void horizontalTransition();
    
    //LEVEL  
    void renderBackground();
    void renderScore();
    void renderTrail();
    void renderObstacles();
    void renderLineObstacle(Vector2 obRenderPosition, int i,Color ob_background);
    void renderBlockObstacle(Vector2 obRenderPosition, int i, Color ob_background);
    void renderParticles();
    Vector2 convertToCameraCoordinates(Vector2 position);

   
    void render();
    void renderPlayer();
    //MENU
    void goToMenu();
    void updateMenuButton();
    void renderMenu(const uint8_t time = 1);
    void menuStartGame();

    ~Game();

};