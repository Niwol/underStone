#include "engine.hpp"

cEngine::cEngine()
    : m_window(nullptr), m_renderer(nullptr), m_screenWidth(0),
      m_screenHeigth(0) {}

bool cEngine::init(const char *title, int width, int heigth, int initFlags,
                   int winFlags, int renderFlags) {

  if (SDL_Init(initFlags) < 0) {
    std::cout << "[ENGINE]: Error, SDL init, SDL error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  if (m_window) {
    std::cout << "[ENGINE]: Warning: window already exsists, recreating it"
              << std::endl;
    SDL_DestroyWindow(m_window);
  }

  m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, heigth, winFlags);

  if (m_window == nullptr) {
    std::cout << "[ENGINE]: Error: window creation, SDL error: "
              << SDL_GetError() << std::endl;

    return false;
  }

  m_screenWidth = width;
  m_screenHeigth = heigth;

  if (m_renderer) {
    std::cout << "[ENGINE]: Warning: renderer already exsists, recreating it"
              << std::endl;
    SDL_DestroyRenderer(m_renderer);
  }

  m_renderer = SDL_CreateRenderer(m_window, 0, renderFlags);
  if (m_renderer == nullptr) {
    std::cout << "[ENGINE]: Error: renderer creation, SDL error: "
              << SDL_GetError() << std::endl;

    return false;
  }

  return true;
}

cEngine::~cEngine() {
  if (m_window)
    SDL_DestroyWindow(m_window);
  if (m_renderer)
    SDL_DestroyRenderer(m_renderer);

  SDL_Quit();
}

void cEngine::start(bool escapeClose) {

  if (setup()) {

    SDL_Event e;

    bool quit = false;

    while (!quit) {
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
          quit = true;

        if (escapeClose && e.type == SDL_KEYDOWN &&
            e.key.keysym.sym == SDLK_ESCAPE)
          quit = true;

        m_events.push_back(e);
      }

      if (!update())
        quit = true;

      m_events.clear();
    }
  }
}
