#include "XCBBackend.hpp"

#include <stdexcept>
#include <array>
#include <sstream>

#include <xcb/xcb.h>

#include "../Debug/console.h"

using namespace YutaniWM;

XCBBackend::XCBBackend() {
  LOG("XCB Backend initializing...");

  connectXCB();
  is_connected = true;

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
  if(is_connected) {
    xcb_disconnect(connection);
    is_connected = false;
  }
}

void XCBBackend::throwAndLogError(const std::string& error_message) {
    ERROR("%s", error_message.c_str());
    disconnectXCB();
    throw std::runtime_error(error_message);
}

void XCBBackend::checkConnectionError() {
  if(xcb_connection_has_error(connection)) {
    throwAndLogError("XCB Connect failed...");
  }
}

void XCBBackend::initialize() {
  setSetup();
  setScreen();
  initializeEvents();
  initializeRoot();
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
  initializeMouseButton(XCB_BUTTON_INDEX_1);
  initializeMouseButton(XCB_BUTTON_INDEX_2);
  initializeMouseButton(XCB_BUTTON_INDEX_3);
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

void XCBBackend::initializeRoot() {
  using std::array;
  using std::stringstream;

  static array<uint32_t, 1> values{{XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                    XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                                    XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY}};
  auto mask = XCB_CW_EVENT_MASK;

  cookie = xcb_change_window_attributes_checked(
      connection, screen->root, mask, values.data());

  auto error = xcb_request_check(connection, cookie);

  xcb_flush(connection);

  if(NULL != error) {
    stringstream ss;
    ss << "Can't redirect root window: " << error->error_code << ". Another Window Manager Running?";
    throwAndLogError(ss.str());
  }
}
