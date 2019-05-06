import struct
import json
import sys
import os

# Full path to binary config.
CONFIGB_PATH = os.path.expanduser("~/chromium.layout.bin~")

# Full path to human readable config.
CONFIGH_PATH = os.path.expanduser("~/chromium.layout.conf")

# The order in which fields are stored inside the binary file.
ATTR_ORDER = [
    "BOOKMARK_BAR_HEIGHT", "BOOKMARK_BAR_BUTTON_HEIGHT",
    "BOOKMARK_BAR_NTP_HEIGHT", "HOSTED_APP_MENU_BUTTON_SIZE",
    "HOSTED_APP_PAGE_ACTION_ICON_SIZE", "LOCATION_BAR_BUBBLE_VERTICAL_PADDING",
    "LOCATION_BAR_BUBBLE_FONT_VERTICAL_PADDING", "LOCATION_BAR_BUBBLE_CORNER_RADIUS",
    "LOCATION_BAR_BUBBLE_ANCHOR_VERTICAL_INSET", "LOCATION_BAR_ELEMENT_PADDING",
    "LOCATION_BAR_HEIGHT", "LOCATION_BAR_ICON_SIZE", "TAB_AFTER_TITLE_PADDING",
    "TAB_ALERT_INDICATOR_CAPTURE_ICON_WIDTH", "TAB_ALERT_INDICATOR_ICON_WIDTH",
    "TAB_HEIGHT", "TAB_PRE_TITLE_PADDING", "TAB_STACK_DISTANCE", "TABSTRIP_TOOLBAR_OVERLAP",
    "TOOLBAR_ELEMENT_PADDING", "TOOLBAR_STANDARD_SPACING", "LOCATION_BAR_ICON_INTERIOR_PADDING_TOP",
    "LOCATION_BAR_ICON_INTERIOR_PADDING_LEFT", "LOCATION_BAR_ICON_INTERIOR_PADDING_BOTTOM",
    "LOCATION_BAR_ICON_INTERIOR_PADDING_RIGHT", "TOOLBAR_BUTTON_TOP", "TOOLBAR_BUTTON_LEFT",
    "TOOLBAR_BUTTON_BOTTOM", "TOOLBAR_BUTTON_RIGHT", "TOOLBAR_ACTION_VIEW_TOP",
    "TOOLBAR_ACTION_VIEW_LEFT", "TOOLBAR_ACTION_VIEW_BOTTOM", "TOOLBAR_ACTION_VIEW_RIGHT"
    "LOCATION_BAR_CHILD_INTERIOR_PADDING", "TOOLBAR_INTERIOR_MARGIN_TOP", "TOOLBAR_INTERIOR_MARGIN_LEFT",
    "TOOLBAR_INTERIOR_MARGIN_BOTTOM", "TOOLBAR_INTERIOR_MARGIN_RIGHT"
]

parsed_map = None

# Check if human readable configuration file exists.
if not os.path.isfile(CONFIGH_PATH):
    print("Could not find file %s." % (CONFIGH_PATH))
    print("Please run the unpacker at least once, before running this script!")
    sys.exit(1)

# Read map from human readable configuration file.
with open(CONFIGH_PATH, "rt") as f:
    parsed_map = json.load(f)

binary_data = bytearray()

# Pack data.
for i in range(len(ATTR_ORDER)):
    subdata = struct.pack("i", parsed_map[ATTR_ORDER[i]])
    binary_data += subdata

# Finally write to binary file.
with open(CONFIGB_PATH, "wb") as f:
    f.write(binary_data)
