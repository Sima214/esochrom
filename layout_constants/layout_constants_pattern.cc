#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string>
extern "C" {
#define INI_IMPLEMENTATION
#include "ini.h"
}

static const char* CONFIG_NAME = "/.config/chromium/layout.ini";

#if defined(OS_MACOSX)
#define COCOA_LAYOUT_NAME "CocoaLayoutConstant"
static const char* COCOA_LAYOUT_LABELS[] = {
  /* CocoaLayoutLabels */
};
#define COCOA_LAYOUT_LENGTH (sizeof(COCOA_LAYOUT_LABELS) / sizeof(char*))
#else
#define COCOA_LAYOUT_LENGTH 0
#endif

#define LAYOUT_NAME "LayoutConstant"
static const char* LAYOUT_LABELS[] = {
  /* LayoutLabels */
};
#define LAYOUT_LENGTH (sizeof(LAYOUT_LABELS) / sizeof(char*))

#define LAYOUT_INSETS_NAME "LayoutInsets"
static const char* LAYOUT_INSETS_LABELS[] = {
  /* LayoutInsetsLabels */
};
#define LAYOUT_INSETS_LENGTH (sizeof(LAYOUT_INSETS_LABELS) / sizeof(char*))

#define LAYOUT_TOTAL_LENGTH (COCOA_LAYOUT_LENGTH + LAYOUT_LENGTH + LAYOUT_INSETS_LENGTH)

static int loaded_layout[LAYOUT_TOTAL_LENGTH] = {};

#define INI_DO_SECTION(name, length, labels, offset)                              \
  section = ini_find_section(ini, name, 0);                                       \
  if (section != INI_NOT_FOUND) {                                                 \
    for (int i = 0; i < length; i++) {                                            \
      int index = ini_find_property(ini, section, labels[i], 0);                  \
      if (index != INI_NOT_FOUND) {                                               \
        int value = std::atoi(ini_property_value(ini, section, index));           \
        loaded_layout[offset + i] = value;                                        \
      }                                                                           \
      else {                                                                      \
        std::printf("LayoutConstants: Couldn't load `%s::%s`!", name, labels[i]); \
      }                                                                           \
    }                                                                             \
  }                                                                               \
  else {                                                                          \
    std::printf("LayoutConstants: Couldn't find `%s` section!\n", name);          \
  }

static void load_layout_constants() {
  std::string config_name(CONFIG_NAME);
  std::string home_path(getenv("HOME"));
  std::string final_path = home_path + config_name;
  std::ifstream config(final_path.c_str(), std::ios::in);
  if(config.good()) {
    // Config file exists.
    config.seekg(0, std::ios::end);
    size_t filesize = config.tellg();
    std::string contents(filesize, '\n');
    config.seekg(0);
    config.read(&contents[0], filesize); 
    config.close();
    // Parse ini properties.
    ini_t* ini = ini_load(contents.c_str(), NULL);
    int section = INI_NOT_FOUND;
    #if defined(OS_MACOSX)
    // CocoaLayoutConstant
    INI_DO_SECTION(COCOA_LAYOUT_NAME, COCOA_LAYOUT_LENGTH, COCOA_LAYOUT_LABELS, 0);
    #endif
    // LayoutConstant
    INI_DO_SECTION(LAYOUT_NAME, LAYOUT_LENGTH, LAYOUT_LABELS, COCOA_LAYOUT_LENGTH);
    // LayoutInsets
    INI_DO_SECTION(LAYOUT_INSETS_NAME, LAYOUT_INSETS_LENGTH, LAYOUT_INSETS_LABELS, COCOA_LAYOUT_LENGTH + LAYOUT_LENGTH);
    ini_destroy(ini);
  } else {
    std::printf("LayoutConstants: couldn't load %s\n", final_path.c_str());
    return;
  }
}

__attribute__((constructor))
static void init_layout_constants() {
  std::fill_n(loaded_layout, LAYOUT_TOTAL_LENGTH, 16);
  load_layout_constants();
}

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/layout_constants.h"

#include "base/logging.h"
#include "ui/base/material_design/material_design_controller.h"

#if defined(OS_MACOSX)
int GetCocoaLayoutConstant(LayoutConstant constant) {
  size_t offset = 0;
  switch (constant) {
    /* CocoaLayoutConstant */
    default:
      return GetLayoutConstant(constant);
  }
}
#endif

int GetLayoutConstant(LayoutConstant constant) {
  size_t offset = COCOA_LAYOUT_LENGTH;
  switch (constant) {
    /* LayoutConstant */
    default:
      break;
  }
  NOTREACHED();
  return 0;
}

gfx::Insets GetLayoutInsets(LayoutInset inset) {
  size_t offset = COCOA_LAYOUT_LENGTH + LAYOUT_LENGTH;
  switch (inset) {
    /* LayoutInsets */
  }
  NOTREACHED();
  return gfx::Insets();
}
