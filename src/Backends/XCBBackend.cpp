#include "XCBBackend.hpp"

#include <stdexcept>
#include <xcb/xcb.h>

#include "../Debug/console.h"

using namespace YutaniWM;

XCBBackend::XCBBackend() {
  LOG("XCB Backend initializing...");

  connectXCB();

  LOG("XCB %d.%d connected... Screen %d",
      setup->protocol_major_version,
      setup->protocol_minor_version, screen_id);

  initialize();
}

XCBBackend::~XCBBackend() {
  LOG("Disconnecting XCB...");
  disconnectXCB();
}

void XCBBackend::connectXCB() {
  connection = xcb_connect(NULL, &screen_id);
  checkConnectionError();
}

void XCBBackend::disconnectXCB() noexcept {
  xcb_disconnect(connection);
}

void XCBBackend::throwAndLogError(const std::string& error_message) const {
    ERROR("%s", error_message.c_str());
    throw std::runtime_error(error_message);
}

void XCBBackend::checkConnectionError() const {
  if(xcb_connection_has_error(connection)) {
    throwAndLogError("XCB Connect failed...");
  }
}

void XCBBackend::initialize() {
  setSetup();
  setScreen();
  initializeEvents();
}

void XCBBackend::setSetup() {
  setup = xcb_get_setup(connection);
  if(!setup) {
    throwAndLogError("Could not get XCB setup.");
  }
}

void XCBBackend::setScreen() {
  auto iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
  for(int i = 0; i < screen_id; ++i) {
    xcb_screen_next(&iter);
  }

  screen = iter.data;
  if(!screen) {
    disconnectXCB();
    static const std::string error_message("XCB Connect failed...");
    throwAndLogError(error_message);
  }

  auto root = screen->root;

  DEBUG_M("Screen size: %dx%d, Root window id: %d",
      screen->width_in_pixels,
      screen->height_in_pixels,
      screen->root);
}

void XCBBackend::initializeEvents() {
  initializeMouseButtons();
}

void XCBBackend::initializeMouseButtons() {
  initializeMouseButton(1);
  initializeMouseButton(2);
  initializeMouseButton(3);
}

void XCBBackend::initializeMouseButton(const int button) {
  xcb_grab_button(connection,
                  0,
                  screen->root,
                  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC,
                  XCB_GRAB_MODE_ASYNC,
                  screen->root,
                  XCB_NONE,
                  button,
                  XCB_MOD_MASK_1);
}
