#include "game.hpp"
#include "string.hpp" //DRAW STRING

int toInt(float f)
{ // To avoid static_cast<int> everywhere
  return static_cast<int>(f);
}

int abs(int x)
{
  return x < 0 ? -x : x;
}

int clamp(int value, int min, int max)
{
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

int generateRandomNumber(int a, int b)
{
  // récupère un nombre aléatoire 32 bits
  uint32_t r = EADK::random();

  // ramène dans l’intervalle [a, b]
  int range = b - a + 1;
  return a + (r % range);
}

// TO OPTIMISE
int generateRandomNumberFarFrom(int a, int b, int far_from, int min_distance)
{
  int num = generateRandomNumber(a, b);
  if (abs(num - far_from) >= min_distance)
  {
    return num;
  }

  if (generateRandomNumber(0, 1) == 0)
  {

    return clamp(num + (min_distance - abs(num - far_from)), a, b);
  }
  else
  {

    return clamp(num - (min_distance - abs(num - far_from)), a, b);
  }
}

bool collideSquareCenter(Vector2 p1, uint16_t size1, Vector2 p2, uint16_t size2)
{
  float dx = p1.x - p2.x;
  float dy = p1.y - p2.y;

  float sumHalf = (size1 + size2) * 0.5f;

  return (dx < sumHalf && dx > -sumHalf &&
          dy < sumHalf && dy > -sumHalf);
}

bool collideRectCenter(
    Vector2 p1, uint16_t width1, uint16_t height1,
    Vector2 p2, uint16_t width2, uint16_t height2)
{
  float dx = p1.x - p2.x;
  float dy = p1.y - p2.y;

  float sumHalfWidth = (width1 + width2) * 0.5f;
  float sumHalfHeight = (height1 + height2) * 0.5f;

  return (dx < sumHalfWidth && dx > -sumHalfWidth &&
          dy < sumHalfHeight && dy > -sumHalfHeight);
}

void intToCharFixed(int value, char *buffer, int digits)
{
  buffer[digits] = '\0';

  for (int i = digits - 1; i >= 0; --i)
  {
    buffer[i] = '0' + (value % 10);
    value /= 10;
  }
}

// Approximation rapide de ln(x) pour x > 0
float ln(float x)
{
  if (x <= 0.0f)
    return -1e30f; // erreur simple pour <=0

  float result = 0.0f;
  while (x >= 2.0f)
  {
    x *= 0.5f;
    result += 0.69314718f;
  } // ln(2)
  while (x <= 0.5f)
  {
    x *= 2.0f;
    result -= 0.69314718f;
  }

  x = x - 1.0f;
  float term = x;
  float sum = 0.0f;
  int n = 1;

  // série de Taylor ln(1+x) = x - x^2/2 + x^3/3 - x^4/4 + ...
  while (n <= 10)
  {
    sum += term / n;
    term *= -x;
    n++;
  }

  return result + sum;
}

float log10(float x)
{
  return ln(x) / 2.30258509f; // ln(10)
}

void saveHighScore(const char *filename, int score)
{
  // Erase the old file if it exists
  extapp_fileErase(filename);
  // Then write the new score
  extapp_fileWrite(filename, (const char *)&score, sizeof(int));
}

// Lire le high score (retourne 0 si pas de sauvegarde)
int loadHighScore(const char *filename)
{
  size_t len = 0;
  const char *data = extapp_fileRead(filename, &len);

  if (data == nullptr || len < sizeof(int))
  {
    return 0; // Pas de sauvegarde trouvée
  }

  int score = 0;
  memcpy(&score, data, sizeof(int));
  return score;
}

Color blendColor(const Color &c1, const Color &c2, float alpha)
{
  Color result;
  result.r = c1.r * alpha + c2.r * (1.0f - alpha);
  result.g = c1.g * alpha + c2.g * (1.0f - alpha);
  result.b = c1.b * alpha + c2.b * (1.0f - alpha);
  return result;
}

Game::Game()
{
  // Initialize player trail
  for (int i = 0; i < TrailLength; ++i)
  {
    player_trail[i] = player_position;
  }
  // Initialize obstacles
  for (int i = 0; i < ObstacleMax; ++i)
  {
    obstacles[i] = Obstacle();
  }

  goToMenu();
}

void Game::startLevel()
{
  // Start game implementation
  player_position = Vector2{0.0f, 120.0f};
  // player_velocity = Vector2{320.0f / TPS, 320.0f / TPS};
  score = 0;

  has_jumped = false;
  jump_counter = 0;

  switch (start_difficulty_chosen)
  {
  case 1:
    difficulty = 0;
    high_score = loadHighScore(EASY_BEST_FILE);
    break;
  case 2:
    difficulty = 150;
    high_score = loadHighScore(MEDIUM_BEST_FILE);
    break;
  case 3:
    difficulty = 1000;
    high_score = loadHighScore(HARD_BEST_FILE);
    break;

  default:
    difficulty = 0;
    break;
  }

  clearObstacles();
  clearTrail();
  clearParticles();

  // PARTICLES
  next_particle_player_x = 320;

  // LEVEL
  old_level_type = RANDOM;
  current_level_type = RANDOM;
  next_level_type = DIAGONAL;
  // old_level_type_x = 0;
  current_level_type_x_started = 0;
  next_level_type_x = 1000;

  // OBSTACLES
  next_obstacle_position = {320, 100};
  diagonal_direction = 1;

  background_alpha = 0.1f;
  obstacles_color_alpha = 0.1f;

  startBlockRandomLevel();
}

void Game::clearObstacles()
{
  for (int i = 0; i < ObstacleMax; ++i)
  {
    obstacles[i] = Obstacle();
  }
}

void Game::clearParticles()
{
  for (int i = 0; i < ParticleMax; ++i)
  {
    particles[i] = Particle();
  }
}

void Game::clearTrail()
{
  for (int i = 0; i < TrailLength; ++i)
  {
    player_trail[i] = Vector2();
  }
}
// void Game::drawRect(float x, float y, float width, float height, int color) {
//     EADK::Rect playerRect(toInt(x), toInt(y), toInt(height), toInt(color));
//     EADK::Display::pushRectUniform(playerRect2, Black);
// }

Vector2 Game::convertToCameraCoordinates(Vector2 position)
{
  return position - camera_position;
}

// RENDER #################################################

void Game::renderPlayer()
{ // TO OPTIMISE DO RELATIVE POSITION SIZE
  Vector2 renderPosition = convertToCameraCoordinates(player_position);
  EADK::Rect playerRect(toInt(renderPosition.x - player_size.x / 2), toInt(renderPosition.y - player_size.y / 2), player_size.x, player_size.y);
  EADK::Display::pushRectUniform(playerRect, Black);

  // EADK::Rect playerRect2(toInt(renderPosition.x - 5) + 3, toInt(renderPosition.y - 5) + 3 * a, 10, 10);
  // EADK::Display::pushRectUniform(playerRect2, Black);

  // renderPosition = convertToCameraCoordinates(player_position);
  // EADK::Rect playerRect3(toInt(renderPosition.x - 3.5) + 6, toInt(renderPosition.y - 3.5) + 6 * a, toInt(7), toInt(7));
  // EADK::Display::pushRectUniform(playerRect3, Black);

  short int a = (player_velocity.y > 0) ? 1 : -1;

  EADK::Rect playerRect2(toInt(renderPosition.x - 4) + 3, toInt(renderPosition.y - 4) + 3 * a, 8, 8);
  EADK::Display::pushRectUniform(playerRect2, Black);

  EADK::Rect playerRect3(toInt(renderPosition.x - 6) - 3, toInt(renderPosition.y - 6) - 3 * a, 12, 12);
  EADK::Display::pushRectUniform(playerRect3, Black);
}

void Game::renderTrail()
{
  Vector2 trailRenderPosition;

  // Skip the first due to skin

  for (int i = 1; i < TrailLength - 1; ++i)
  { //- 1 is to avoid glitch
    if (player_trail[i].x == 0)
    {
      continue;
    }
    trailRenderPosition = convertToCameraCoordinates(player_trail[i]);
    EADK::Rect trailRect(toInt(trailRenderPosition.x - trail_size / 2), toInt(trailRenderPosition.y - trail_size / 2), toInt(trail_size), toInt(trail_size));
    EADK::Display::pushRectUniform(trailRect, 0xFFFFFF);
  }

  // for (int i = 0; i < TrailLength -1 ; ++i) {
  //     if (player_trail[i].x == 0) {continue;}
  //     trailRenderPosition = convertToCameraCoordinates(player_trail[i]);
  //     EADK::Rect trailRect2(toInt(trailRenderPosition.x - trail_inside_size.x/2), toInt(trailRenderPosition.y - trail_inside_size.y/2), trail_inside_size.x, trail_inside_size.y);
  //     EADK::Display::pushRectUniform(trailRect2, 0xFFFFFF);
  // }
}

// OBSTACLES
void Game::renderObstacles()
{
  Vector2 obRenderPosition;
  // bool is_dispawn_animation = false;
  Vector2u_int16 new_size = ob_size;
  Vector2u_int16 new_size_inside = ob_inside_size;
  uint8_t hole_y_size;
  // COLOR
  Color ob_background;
  if (current_statue == DEATH)
  {
    ob_background = Color(255, 255, 255); // Color(150, 0, 0);
  }
  else
  {
    ob_background = Color(200, 200, 200);
    // if (obstacles_color_alpha >= 1.f)
    // {
    //   delta_obstacles_color_alpha = -delta_obstacles_color_alpha;
    // }
    // else if (obstacles_color_alpha <= 0)
    // {
    //   delta_obstacles_color_alpha = -delta_obstacles_color_alpha;
    // }
    // obstacles_color_alpha += delta_obstacles_color_alpha;

    // ob_background = blendColor(obstacles_color1, obstacles_color2, obstacles_color_alpha);
  }

  for (int i = 0; i < ObstacleMax; ++i)
  {
    new_size = ob_size;
    if (!obstacles[i].real)
    {
      continue;
    }

    obRenderPosition = convertToCameraCoordinates(obstacles[i].position);
    // int x_size =ob_size.x
    // int y_size =ob_size.y

    // is_dispawn_animation = false;
    if (current_statue == DEATH)
    {
      float dist_to_player = abs(toInt(player_position.x) - toInt(obstacles[i].position.x));
      float alpha = 1.0f - dist_to_player / 270.0f;

      if (alpha < 0)
        alpha = 0;
      if (alpha > 1)
        alpha = 1;
      ob_background = blendColor(Color(255, 240, 240), Color(117, 0, 0), alpha);
    }

    if (obstacles[i].type == BLOCK)
    {
      renderBlockObstacle(obRenderPosition, i, ob_background);
    }
    else
    {
      renderLineObstacle(obRenderPosition, i, ob_background);
      //  EADK::Rect obRect(toInt(obRenderPosition.x - 12 / 2), toInt(obRenderPosition.y -  obstacles[i].hole_y_size/ 2), 12, obstacles[i].hole_y_size);
      // EADK::Display::pushRectUniform(obRect, ob_background.hex());

      // EADK::Rect obRect2(toInt(obRenderPosition.x - 10/ 2), toInt(obRenderPosition.y -  obstacles[i].hole_y_size/ 2), 10, obstacles[i].hole_y_size);
      // EADK::Display::pushRectUniform(obRect2, Black);
    }
  }
}

void Game::renderBlockObstacle(Vector2 obRenderPosition, int i, Color ob_background)
{
  Vector2u_int16 new_size = ob_size;
  Vector2u_int16 new_size_inside = ob_inside_size; // TO OPTIMISE
  if (obRenderPosition.x > x_end_spawn_animation)
  {

    float alpha = 1 - (obRenderPosition.x - x_end_spawn_animation) / (320 - x_end_spawn_animation);

    if (obRenderPosition.y > 120)
    {
      // obRenderPosition.y = obRenderPosition.y +(230 * (1- obstacles[i].alpha));
      obRenderPosition.y = obRenderPosition.y * alpha + (240 * (1 - alpha));
    }
    else
    {
      obRenderPosition.y = obRenderPosition.y * alpha;
    }

    // else
    // {
    //   hole_y_size = toInt(obstacles[i].hole_y_size * alpha);
    // }
  }
  else if (obRenderPosition.x < x_start_disspawn_animation)
  {
    // 30 - > 0
    // 1 -> 0

    float alpha = (obRenderPosition.x) / (x_start_disspawn_animation);
    // is_dispawn_animation = true;
    new_size.x = toInt(new_size.x * alpha);
    new_size.y = toInt(new_size.y * alpha);
    new_size_inside.x = toInt(new_size_inside.x * alpha);
    new_size_inside.y = toInt(new_size_inside.y * alpha);

    // else {
    //     hole_y_size = obstacles[i].hole_y_size;
    // }
    //  x_size = toInt(x_size*alpha);
    //   y_size = toInt(y_size*alpha);

    // if (obRenderPosition.y > 120)
    // {
    //   // obRenderPosition.y = obRenderPosition.y +(230 * (1- obstacles[i].alpha));
    //   obRenderPosition.y = obRenderPosition.y * alpha + (220 * (1 - alpha));
    // }
    // else
    // {
    //   obRenderPosition.y = obRenderPosition.y * alpha;
    // }
  }
  // else {
  //    hole_y_size = obstacles[i].hole_y_size;
  // }
  // BUG CORRECTION

  if (obRenderPosition.y < 0)
  {
    obRenderPosition.y = 0;
  }
  else if (obRenderPosition.y > 240)
  {
    obRenderPosition.y = 240;
  }

  // COLOR

  EADK::Rect obRect(toInt(obRenderPosition.x - new_size.x / 2), toInt(obRenderPosition.y - new_size.y / 2), new_size.x, new_size.y);
  EADK::Display::pushRectUniform(obRect, ob_background.hex());

  EADK::Rect obRect2(toInt(obRenderPosition.x - new_size_inside.x / 2), toInt(obRenderPosition.y - new_size_inside.y / 2), new_size_inside.x, new_size_inside.y);
  EADK::Display::pushRectUniform(obRect2, Black);
}

void Game::renderLineObstacle(Vector2 obRenderPosition, int i, Color ob_background)
{
  float alpha = 1;
  if (obRenderPosition.x > x_end_spawn_animation)
  {

    alpha = 1 - (obRenderPosition.x - x_end_spawn_animation) / (WIDTH - x_end_spawn_animation);
    if (alpha < 0)
      alpha = 0;
    if (alpha > 1)
      alpha = 1;
  }

  float hole_y_size = obstacles[i].hole_y_size * alpha + (235 * (1 - alpha));
  int hole_i = toInt(hole_y_size);

  int first_height = toInt(obRenderPosition.y - hole_i / 2);
  if (first_height < 0)
    first_height = 0;

  int y_second = first_height + hole_i;

  if (y_second > HEIGHT)
    y_second = HEIGHT;

  int second_height = HEIGHT - y_second;
  if (second_height < 0)
    second_height = 0;

  int x_line = toInt(obRenderPosition.x - ob_line_x_size / 2);

  if (first_height > diagonal_error_correction)
  {

    // TOP PART
    EADK::Rect obRectTop(x_line, first_height - 2, ob_line_x_size, 2);
    EADK::Display::pushRectUniform(obRectTop, ob_background.hex());
    // inside
    EADK::Rect obRectTop2(x_line, 0, ob_line_x_size, first_height - 2);
    EADK::Display::pushRectUniform(obRectTop2, Black);
    // EADK::Display::pushRectUniform(obRectTop2, Black);
  }

  if (y_second > diagonal_error_correction && second_height > diagonal_error_correction)
  {
    // BOTTOM PART

    EADK::Rect obRectBottom(x_line, y_second, ob_line_x_size, 2);
    EADK::Display::pushRectUniform(obRectBottom, ob_background.hex());

    EADK::Rect obRectBottom2(x_line, y_second + 2, ob_line_x_size, second_height - 2); // ,+2, ,-2
    EADK::Display::pushRectUniform(obRectBottom2, Black);
    // EADK::Display::pushRectUniform(obRectBottom2, Black);
  }
  if (!ob_corruption)
  {

    EADK::Rect voidRect(x_line, first_height, ob_line_x_size, toInt(hole_y_size));
    EADK::Display::pushRectUniform(voidRect, current_background);
  }

  //   if (current_level_type== RANDOM) {

  //   if (first_height > 0) {

  //     //TOP PART
  //     //outside
  //     EADK::Rect obRectTop(toInt(obRenderPosition.x - ob_line_x_size / 2),  first_height -2, ob_line_x_size, 2);
  //     EADK::Display::pushRectUniform(obRectTop, ob_background.hex());
  //     //inside
  //     EADK::Rect obRectTop2(toInt(obRenderPosition.x - ob_line_x_size / 2), 0, ob_line_x_size, first_height -2);
  //     EADK::Display::pushRectUniform(obRectTop2, Black);
  //   }

  //  if (y_second > 0 && second_height > 0) {
  //    //BOTTOM PART
  //   EADK::Rect obRectBottom(toInt(obRenderPosition.x -  ob_size.x / 2), y_second,  ob_size.x, second_height);
  //   EADK::Display::pushRectUniform(obRectBottom, ob_background.hex());

  //   EADK::Rect obRectBottom2(toInt(obRenderPosition.x - ob_inside_size.x / 2), y_second +2, ob_inside_size.x, second_height-2);
  //   EADK::Display::pushRectUniform(obRectBottom2, Black);

  //  }

  // }
  // else {
  //   if (first_height > 0) {

  //     //TOP PART
  //     EADK::Rect obRectTop(toInt(obRenderPosition.x - ob_size.x / 2), 0, ob_lin.x, first_height);
  //     EADK::Display::pushRectUniform(obRectTop, ob_background.hex());

  //     EADK::Rect obRectTop2(toInt(obRenderPosition.x - ob_size.x / 2), 0, ob_size.x, first_height -2);
  //     EADK::Display::pushRectUniform(obRectTop2, Black);
  //   }

  //  if (y_second > 0 && second_height > 0) {
  //    //BOTTOM PART
  //   EADK::Rect obRectBottom(toInt(obRenderPosition.x -  ob_size.x / 2), y_second,  ob_size.x, second_height);
  //   EADK::Display::pushRectUniform(obRectBottom, ob_background.hex());

  //   EADK::Rect obRectBottom2(toInt(obRenderPosition.x - ob_inside_size.x / 2), y_second +2, ob_inside_size.x, second_height-2);
  //   EADK::Display::pushRectUniform(obRectBottom2, Black);

  //  }
  // }
}

// GUI
void Game::renderScore()
{
  if (score < high_score && high_score - score < 5 && high_score > 10)
  { // When close to high -> Yellow
    renderNumber(150, 5, score, Yellow);
  }
  else
  {
    renderNumber(150, 5, score, White);
  }
}

void Game::renderBackground()
{
  if (current_statue == DEATH)
  {
    current_background = 0xee3333;
  }

  else
  {

    if (background_alpha >= 1.f)
    {
      delta_background_alpha = -delta_background_alpha;
    }
    else if (background_alpha <= 0)
    {
      delta_background_alpha = -delta_background_alpha;
    }
    background_alpha += delta_background_alpha;
    current_background = blendColor(background1, background2, background_alpha).hex();
  }

  if (current_level_type == DIAGONAL && current_statue == LEVEL)
  {
    int screenStart = current_level_type_x_started - camera_position.x;

    if (screenStart < 0)
      screenStart = 0;
    if (screenStart > WIDTH)
      screenStart = WIDTH;

    if (screenStart > 0)
    {
      EADK::Rect rectClear(0, 0, screenStart, HEIGHT);
      EADK::Display::pushRectUniform(rectClear, current_background);
    }
  }
  else if (old_level_type == DIAGONAL && current_statue == LEVEL)
  {
    int screenStart = current_level_type_x_started - camera_position.x;

    if (screenStart < 0)
    {
      // plus rien à protéger
      EADK::Display::pushRectUniform(EADK::Screen::Rect, current_background);
    }
    else if (screenStart < WIDTH)
    {
      EADK::Rect rectClear(screenStart, 0, WIDTH - screenStart, HEIGHT);
      EADK::Display::pushRectUniform(rectClear, current_background);
    }
  }
  else
  {
    EADK::Display::pushRectUniform(EADK::Screen::Rect, current_background);
  }
}

void Game::renderParticles()
{

  Color blended_particle_color = blendColor(particle_color_initial, Color(current_background), 0.5f);
  for (int i = 0; i < ParticleMax; ++i)
  {
    if (particles[i].real)
    {
      // Convert particle screen position to world position
      float particle_world_x = particles[i].position.x + camera_position.x;

      // Skip particles in diagonal level zones
      if (current_level_type == DIAGONAL && particle_world_x >= current_level_type_x_started)
      {
        continue;
      }
      if (old_level_type == DIAGONAL && particle_world_x < current_level_type_x_started)
      {
        continue;
      }

      EADK::Rect particleRect(toInt(particles[i].position.x - particles[i].size / 40 / 2), toInt(particles[i].position.y - particles[i].size / 40 / 2), particles[i].size / 40, particles[i].size / 40);
      EADK::Display::pushRectUniform(particleRect, blended_particle_color.hex()); // TO OPTIMISE, we can precalculate the blended color and just change its alpha, but it will be more expensive in term of calculation, so we will do it like this for now
    }
  }
}

void Game::render()
{
  camera_position.x = player_position.x - camera_offset.x;

  renderBackground();
  renderParticles();

  renderObstacles();
  // renderScore();
  renderTrail();
  renderPlayer();
  renderScore();

  // renderNumber(180, 5, toInt(player_position.x), White);
  // renderNumber(20, 5, toInt(difficulty), White);
  // renderNumber(70, 5, toInt(250 - toInt(45 * log10(difficulty + 1))), White);
}

// UPDATE ##########################################

void Game::handleInput()
{

  EADK::Keyboard::State keyboardState = EADK::Keyboard::scan();
  if (keyboardState.keyDown(EADK::Keyboard::Key::Shift))
  {
    quitGame();
    return;
  }

  if (keyboardState.keyDown(EADK::Keyboard::Key::Four))
  {
    reset_counter++;
    if (reset_counter >= 40)
    {
      // TO CRASH
      int *ptr = nullptr;
      *ptr = 42;
    }
  }
  if (current_statue == LEVEL)
  {

    if (keyboardState.keyDown(EADK::Keyboard::Key::OK))
    {
      player_velocity.y = -player_real_vy;
      if (!has_jumped)
      {
        jump_counter++;
        has_jumped = true;
      }
    }
    else
    {
      player_velocity.y = player_real_vy;
      has_jumped = false;
    }

    if (keyboardState.keyDown(EADK::Keyboard::Key::Back))
    {
      goToPause();
      EADK::Timing::msleep(200);
    }
  }

  else if (current_statue == PAUSE)
  {
    if (keyboardState.keyDown(EADK::Keyboard::Key::Back) || keyboardState.keyDown(EADK::Keyboard::Key::OK))
    {
      quitPauseMode();
      EADK::Timing::msleep(200);
    }
  }
  else if (current_statue == DEATH)
  {

    if (keyboardState.keyDown(EADK::Keyboard::Key::OK))
    {
      respawn();
    }
    if (keyboardState.keyDown(EADK::Keyboard::Key::Backspace))
    {
      goToMenu();
    }
  }
  else if (current_statue == MENU)
  {
    if (keyboardState.keyDown(EADK::Keyboard::Key::OK))
    {
      menuStartGame();
    }

    if (keyboardState.keyDown(EADK::Keyboard::Key::Right))
    {
      if (start_difficulty_chosen == 3)
      {
        start_difficulty_chosen = 1;
      }
      else
      {
        start_difficulty_chosen++;
      }
      updateMenuButton();
      EADK::Timing::msleep(200);
    }
    if (keyboardState.keyDown(EADK::Keyboard::Key::Left))
    {
      if (start_difficulty_chosen == 1)
      {
        start_difficulty_chosen = 3;
      }
      else
      {
        start_difficulty_chosen--;
      }
      updateMenuButton();
      EADK::Timing::msleep(200);
    }
  }
}

void Game::quitGame()
{
  quit = true;
  clearObstacles();
  clearTrail();
}

// LEVEL
void Game::updateLevel()
{
  if (next_level_type_x < player_position.x + 320)
  {
    old_level_type = current_level_type;
    current_level_type = next_level_type;
    current_level_type_x_started = next_level_type_x;
    if (current_level_type == DIAGONAL)
    {
      startDiagonalLevel();
    }
    else
    {
      startBlockRandomLevel();
    }

    if (current_level_type == RANDOM)
    {
      next_level_type = DIAGONAL;
    }
    else
    {
      next_level_type = RANDOM;
    }
  }
}

void Game::startBlockRandomLevel()
{

  next_level_type_x = player_position.x + generateRandomNumber(2000, 3000); // DURATION
  // next_level_type_x = player_position.x + generateRandomNumber(1000, 1000); // DURATION

  next_obstacle_position = {player_position.x + 400, 100};
}

void Game::startDiagonalLevel()
{

  next_level_type_x = player_position.x + generateRandomNumber(900, 1400); // DURATION
  next_obstacle_position = {player_position.x + 320, 100};
  straight_line_duration = 0;
  diagonal_direction = 1;
  int pxmax = 210 - toInt(35 * log10(difficulty + 1));
  int pxmin = 210 - toInt(45 * log10(difficulty + 1));
  //  int pxmax = 250 - toInt(45 * log10(difficulty + 1));
  //  int pxmin = 130 - toInt(15 * log10(difficulty + 1));
  spacing = generateRandomNumber(pxmin, pxmax);

  // line_spawn_max_y = 250 - toInt(40 * log10(difficulty + 1));
  // line_spawn_min_y = 20 + toInt(15 * log10(difficulty + 1));
  line_spawn_max_y = 190 - toInt(22 * log10(difficulty + 1));
  line_spawn_min_y = 90 - toInt(5 * log10(difficulty + 1));
}

// OBSTALES
void Game::addObstacle(float x, float y)
{
  for (int i = ObstacleMax - 1; i > 0; --i)
  {
    obstacles[i] = obstacles[i - 1];
  }

  obstacles[0] = Obstacle(x, y);
}

void Game::addObstacleLine(float x, float y, uint8_t hole_size)
{
  for (int i = ObstacleMax - 1; i > 0; --i)
  {
    obstacles[i] = obstacles[i - 1];
  }

  obstacles[0] = Obstacle(x, y, hole_size);
}

void Game::updateObstacles()
{
  // To Optimise
  if (current_level_type == RANDOM)
  {
    int pxmax = 200 - toInt(40 * ln(difficulty / 15 + 1));
    int pxmin = 70 - (10 * ln(difficulty / 10 + 1));

    if (pxmin < 0)
      pxmin = 0;
    if (pxmin > pxmax)
      pxmax = pxmin;

    if (player_position.x + 320 > next_obstacle_position.x && (next_level_type_x - player_position.x) > 500)
    {

      int new_y = generateRandomNumberFarFrom(15, 220, next_obstacle_position.y, 25);
      // if (abs(new_y - next_obstacle_position.y) < 40) // ERROR
      // {
      //   renderNumber(10, 5, abs(new_y - next_obstacle_position.y), White);
      // }
      addObstacle(player_position.x + 320, new_y);

      next_obstacle_position = {player_position.x + 320 + generateRandomNumber(pxmin, pxmax), 40};
    }
  }
  else
  {
    if (player_position.x + 320 > next_obstacle_position.x)
    {

      // addObstacle(player_position.x + 320, generateRandomNumber(15, 220), 50, 50);
      addObstacleLine(player_position.x + 320, next_obstacle_position.y, spacing);

      // next_obstacle_x = player_position.x + 320 + generateRandomNumber(pxmin, pxmax);

      if (straight_line_duration > 1)
      {
        straight_line_duration--;
        next_obstacle_position = {player_position.x + 320 + 16, next_obstacle_position.y};
      }
      else if (straight_line_duration == 1)
      {
        straight_line_duration--;
        next_obstacle_position = {player_position.x + 320 + 16, next_obstacle_position.y + 23 * diagonal_direction};
      }
      else
      {
        if (next_obstacle_position.y > line_spawn_max_y)
        {
          if (generateRandomNumber(0, 3) == 0)
          {
            straight_line_duration = 5;
          }
          diagonal_direction = -1;
        }
        else if (next_obstacle_position.y < line_spawn_min_y)
        {
          if (generateRandomNumber(0, 3) == 0)
          {
            straight_line_duration = 5;
          }
          diagonal_direction = 1;
        }
        if (straight_line_duration > 1)
        {

          next_obstacle_position = {player_position.x + 320 + 16, next_obstacle_position.y};
        }
        else
        {

          next_obstacle_position = {player_position.x + 320 + 16, next_obstacle_position.y + 23 * diagonal_direction};
        }
      }
    }
  }
  // CLEARING
  for (int i = 0; i < ObstacleMax; ++i)
  {
    if (obstacles[i].real && obstacles[i].position.x < player_position.x - camera_offset.x)
    {
      obstacles[i] = Obstacle();
      score += 1;
      difficulty += 1;
      continue;
    }
  }
}

// PLAYER
void Game::updateTrail()
{

  if (trail_size > 9)
  {
    adding_trail_size = -0.5f;
  }
  else if (trail_size < 5)
  {
    adding_trail_size = 0.5;
  }
  trail_size += adding_trail_size;

  for (int i = TrailLength - 1; i >= 2; --i)
  {
    player_trail[i] = player_trail[i - 2];
  }

  player_trail[0] = player_position;
  player_trail[1] = player_position - player_velocity * 0.5f;
  // player_trail[2] = player_position - player_velocity * 0.66f;
  //  player_trail[3] = player_position - player_velocity * 0.75f;
}

void Game::updatePlayer()
{
  player_position = player_position + player_velocity;
}

bool Game::isPlayerShouldDie()
{
  for (int i = 0; i < ObstacleMax; ++i)
  {
    if (!obstacles[i].real || obstacles[i].position.x > player_position.x + 20)
      continue;

    if (obstacles[i].type == BLOCK)
    {

      if (collideSquareCenter(player_position, player_size.x, obstacles[i].position, ob_size.x))
      {
        return true;
      }
    }
    else
    {
      int first_height = toInt(obstacles[i].position.y - obstacles[i].hole_y_size / 2);
      Vector2 center_rect = Vector2(obstacles[i].position.x, toInt(first_height / 2));

      if (first_height > diagonal_error_correction)
      {
        if (collideRectCenter(player_position, player_size.x, player_size.y, center_rect, ob_size.x, first_height))
          return true;
      }
      int y_second = (first_height + obstacles[i].hole_y_size);
      int second_height = HEIGHT - y_second;
      if (y_second > diagonal_error_correction && second_height > diagonal_error_correction)
      {

        center_rect = Vector2(obstacles[i].position.x, y_second + toInt(second_height / 2));
        if (collideRectCenter(player_position, player_size.x, player_size.y, center_rect, ob_size.x, second_height))
          return true;
      }
    }
  }

  if (player_position.y < 5)
    return true;

  if (player_position.y > 235)
    return true;

  return false;
}

void Game::respawn()
{
  current_statue = LEVEL;
  startLevel();
}

// PARTICLES
void Game::updateParticles()
{
  // To Optimise

  if (player_position.x + 320 > next_particle_player_x)
  {
    // ADD NEW PARTICLE
    for (int i = ParticleMax - 1; i > 0; --i)
    {
      particles[i] = particles[i - 1];
    }
    //                                            size,                           x,                                y
    particles[0] = Particle(generateRandomNumber(50, 255), generateRandomNumber(50, 320), generateRandomNumber(5, 235));

    next_particle_player_x = player_position.x + 320 + generateRandomNumber(10, 40);
  }

  // CLEARING AND MOVING
  for (int i = 0; i < ParticleMax; ++i)
  {
    particles[i].position.x -= player_velocity.x / 3; // PARALLAX

    particles[i].size -= 1;

    if (particles[i].real && (particles[i].position.x <= 0 || particles[i].size == 0))
    {
      particles[i] = Particle();
      continue;
    }
  }
}

// DIE
void Game::goToDeadStatue()
{
  current_statue = DEATH;
  render();
  EADK::Color color = White;
  EADK::Color difficulty_color = White;
  if (score > high_score)
  {
    high_score = score;
    color = Yellow;

    if (start_difficulty_chosen == 1)
    {
      saveHighScore(EASY_BEST_FILE, high_score);
    }
    else if (start_difficulty_chosen == 2)
    {
      saveHighScore(MEDIUM_BEST_FILE, high_score);
    }
    else if (start_difficulty_chosen == 3)
    {
      saveHighScore(HARD_BEST_FILE, high_score);
    }
  
  }

  if (start_difficulty_chosen == 1)
  {
    difficulty_color = difficulty_1_color;
  }
  else if (start_difficulty_chosen == 2)
  {
    difficulty_color = difficulty_2_color;
  }
  else if (start_difficulty_chosen == 3)
  {
    difficulty_color = difficulty_3_color;
  }

  EADK::Display::pushRectUniform(EADK::Rect(128, 78, 192, 84), White);
  EADK::Display::pushRectUniform(EADK::Rect(130, 80, 190, 80), Black);

  EADK::Display::pushRectUniform(EADK::Rect(315, 80, 5, 80), difficulty_color);

#if STRING
  EADK::Display::drawString("SCORE", EADK::Point(150, 90), true, 0xffffff, 0x000000);
  EADK::Display::drawString("BEST", EADK::Point(150, 110), true, color, 0x000000);
  EADK::Display::drawString("JUMP", EADK::Point(150, 130), true, 0xffffff, 0x000000);
#endif

  renderNumber(250, 90, score, White, 3);
  renderNumber(250, 110, high_score, color, 3);
  renderNumber(250, 130, jump_counter, White, 3);
  EADK::Timing::msleep(200);
}

// PAUSE
void Game::goToPause()
{
  current_statue = PAUSE;

  EADK::Rect pause1(145, 100, 10, 40);
  EADK::Display::pushRectUniform(pause1, White);

  EADK::Rect pause2(165, 100, 10, 40);
  EADK::Display::pushRectUniform(pause2, White);
}

void Game::quitPauseMode()
{
  current_statue = LEVEL;
}

// MENU
void Game::drawLine(int x1, int y1, int x2, int y2, int step, int size, int timestep)
{
  int dx = x2 - x1;
  int dy = y2 - y1;

  int dirX = (dx > 0) ? 1 : -1;
  int dirY = (dy > 0) ? 1 : -1;

  dx = (dx);
  dy = (dy);

  int length = dx; // toujours gauche → droite donc dx >= 0

  for (int i = 0; i <= length; i += step)
  {
    int x = x1 + i * dirX;
    int y = y1;

    // Cas diagonale parfaite
    if (dy != 0)
      y = y1 + i * dirY;

    EADK::Rect r(x - size, y - size, size * 2, size * 2);
    EADK::Display::pushRectUniform(r, 0xffffff);
    EADK::Timing::msleep(timestep);
  }
}

void Game::renderMenu()
{
  // {
  //   EADK::Display::pushRectUniform(EADK::Screen::Rect, 0x000055);
  //   EADK::Display::pushRectUniform(EADK::Screen::Rect, 0x0000AA);
  //   EADK::Display::pushRectUniform(EADK::Screen::Rect, 0x000077);

  int step = 5;

  for (int i = 0; i < step; ++i)
  {
    EADK::Display::pushRectUniform({i * 32, 0, 32, 90 + i * 10}, 0x0000EE);        // gauche
    EADK::Display::pushRectUniform({160 + i * 32, 0, 32, 130 - i * 10}, 0x0000EE); // droite
  }

  const uint8_t size = 6;
  const uint8_t time = 1;
  // TITLE
  // TITLE

  // W
  drawLine(34, 48, 65, 81, 2, size, time);
  drawLine(66, 80, 98, 48, 2, size, time);
  drawLine(98, 48, 130, 80, 2, size, time);
  drawLine(131, 80, 164, 47, 2, size, time);

  // V
  drawLine(164, 47, 196, 80, 2, size, time);
  drawLine(197, 80, 228, 47, 2, size, time);

  // E
  drawLine(229, 48, 261, 80, 2, size, time);

  drawLine(147, 70, 183, 70, 2, size - 1, time);

  drawLine(240, 47, 266, 47, 2, size - 1, time);
  drawLine(252, 64, 276, 64, 2, size - 1, time);
  drawLine(262, 81, 286, 81, 2, size - 1, time);

#if STRING
  // EADK::Display::drawString("WAVE", EADK::Point(145, 50), true, 0xAAAAff, 0x0000AA);
  EADK::Display::drawString("Select start difficulty", EADK::Point(50, 140), true, 0xffffff, 0x0000AA);
#endif

  updateMenuButton();
}

void Game::updateMenuButton()
{

  EADK::Display::pushRectUniform(EADK::Rect(0, 160, 320, 100), 0x000055);
  // EADK::Rect rect = EADK::Rect(x_square_pos, y_square_pos, unit, unit);
  // EADK::Display::pushRectUniform(rect, 0x000000);
  const uint8_t xsize = 76;
  const uint8_t ysize = 36;

  constexpr uint16_t buttonEasyX = 40;
  constexpr uint16_t buttonMediumX = 130;
  constexpr uint16_t buttonHardX = 220;
  constexpr uint16_t buttonY = 180;

  if (start_difficulty_chosen == 1)
  {
    EADK::Rect select(37, buttonY - 3, xsize, ysize);
    EADK::Display::pushRectUniform(select, 0xffffff);
  }
  else if (start_difficulty_chosen == 2)
  {
    EADK::Rect select(127, buttonY - 3, xsize, ysize);
    EADK::Display::pushRectUniform(select, 0xffffff);
  }
  else if (start_difficulty_chosen == 3)
  {
    EADK::Rect select(217, buttonY - 3, xsize, ysize);
    EADK::Display::pushRectUniform(select, 0xffffff);
  }

  EADK::Rect buttonEasy(buttonEasyX, buttonY, 70, 30);
  EADK::Display::pushRectUniform(buttonEasy, difficulty_1_color);
  EADK::Rect buttonMedium(buttonMediumX, buttonY, 70, 30);
  EADK::Display::pushRectUniform(buttonMedium, difficulty_2_color);
  EADK::Rect buttonHard(buttonHardX, buttonY, 70, 30);
  EADK::Display::pushRectUniform(buttonHard, difficulty_3_color);
}

void Game::diagonalTransition()
{
  int unit = 40; // Must be 320 and 240 divisor like 80, 40, 20, 10
  int x_square_pos = 0;
  int y_square_pos = 0;
  int startx_square_pos = 0;
  int starty_square_pos = 0;
  int count = 0; // FOR security
  while (count < 8 * 6)
  {

    if (x_square_pos > WIDTH && y_square_pos > HEIGHT)
    {
      break;
    }
    if (x_square_pos < 0)
    {
      startx_square_pos += unit;
      x_square_pos = startx_square_pos;
      y_square_pos = starty_square_pos;
    }

    if (y_square_pos > HEIGHT)
    {
      startx_square_pos += unit;
      x_square_pos = startx_square_pos;
      y_square_pos = starty_square_pos;
    }

    if (x_square_pos > WIDTH)
    {
      starty_square_pos += unit;

      x_square_pos = startx_square_pos;
      y_square_pos = starty_square_pos;
    }
    EADK::Rect rect = EADK::Rect(x_square_pos, y_square_pos, unit, unit);
    EADK::Display::pushRectUniform(rect, 0x000000);
    EADK::Timing::msleep(3);

    x_square_pos -= unit;
    y_square_pos += unit;

    count++;
  }
}

void Game::drawRectSafe(int x, int y, int sizex, int sizey, int color)
{

  // Si complètement hors écran → on quitte
  if (x >= WIDTH || y >= HEIGHT)
    return;
  if (x + sizex <= 0 || y + sizey <= 0)
    return;

  // Clip à droite
  if (x + sizex > WIDTH)
  {
    sizex = WIDTH - x;
  }

  // Clip en bas
  if (y + sizey > HEIGHT)
  {
    sizey = HEIGHT - y;
  }

  // Clip à gauche
  if (x < 0)
  {
    sizex += x; // x est négatif
    x = 0;
  }

  // Clip en haut
  if (y < 0)
  {
    sizey += y; // y est négatif
    y = 0;
  }

  // Sécurité finale
  if (sizex <= 0 || sizey <= 0)
    return;

  EADK::Display::pushRectUniform(EADK::Rect(x, y, sizex, sizey), color);
}

void Game::diagonalTransition2()
{

  int t = 6;
  int unit = 150;

  // =========================
  // 1️⃣ DIAGONALE MONTANTE ↗
  // =========================

  int x = -unit;
  int y = HEIGHT - unit;

  int dx = 12;
  int dy = -12;

  for (int i = 0; i < 35; i++)
  {
    drawRectSafe(x, y, unit, unit, 0x000000);
    EADK::Timing::msleep(t);
    x += dx;
    y += dy;
  }

  // =========================
  // 2️⃣ DIAGONALE DESCENDANTE ↘
  // =========================

  x = 250;
  y = -unit;

  dx = -12;
  dy = 12;

  for (int i = 0; i < 35; i++)
  {
    drawRectSafe(x, y, unit, unit, 0x000000);
    EADK::Timing::msleep(t);
    x += dx;
    y += dy;
  }

  // =========================
  // 3️⃣ DIAGONALE MONTANTE ↗
  // =========================

  x = 250 - unit;
  y = HEIGHT;

  dx = 12;
  dy = -12;

  for (int i = 0; i < 35; i++)
  {
    drawRectSafe(x, y, unit, unit, 0x000000);
    EADK::Timing::msleep(t);
    x += dx;
    y += dy;
  }
}

void Game::horizontalTransition()
{
  int square_size = 20;
  // int square_column_count = static_cast<int>(320/square_size);
  // int square_line_count = static_cast<int>(240/square_size);

  for (int x = 0; x < 320; x += square_size)
  {
    for (int y = 0; y < 240; y += square_size)
    {
      EADK::Rect rect = EADK::Rect(x, y, square_size, square_size);
      EADK::Display::pushRectUniform(rect, 0x000000);
      EADK::Timing::msleep(5);
    }
  }
}

void Game::goToMenu()
{
  high_score = 0;
  current_statue = MENU;
  EADK::Display::pushRectUniform(EADK::Screen::Rect, 0x0000AA);
  renderMenu();
  EADK::Timing::msleep(500);
}

void Game::menuStartGame()
{
  diagonalTransition2();

  //  horizontalTransition();
  respawn();
}

void Game::update()
{

  // camera_y = player_y + camera_offset_y;
  handleInput();

  if (current_statue == LEVEL)
  {
    updateLevel();
    updateObstacles();
    updatePlayer();
    updateTrail();
    updateParticles();

    render();

    if (isPlayerShouldDie())
    {
      goToDeadStatue();
    };
  }
  else if (current_statue == DEATH)
  {
  }
  else if (current_statue == MENU)
  {
    // renderMenu();
  }
  else if (current_statue == PAUSE)
  {
    // renderMenu();
  }

  EADK::Timing::msleep(MSPT);
}

Game::~Game()
{
}
