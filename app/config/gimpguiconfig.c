/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * GimpGuiConfig class
 * Copyright (C) 2001  Sven Neumann <sven@gimp.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpconfig/gimpconfig.h"

#include "config-types.h"

#include "gimprc-blurbs.h"
#include "gimpguiconfig.h"

#include "gimp-intl.h"


#define DEFAULT_HELP_BROWSER   GIMP_HELP_BROWSER_GIMP
#define DEFAULT_THEME          "Default"

#define DEFAULT_USER_MANUAL_ONLINE_URI \
  "http://docs.gimp.org/" GIMP_APP_VERSION_STRING


enum
{
  PROP_0,
  PROP_MOVE_TOOL_CHANGES_ACTIVE,
  PROP_IMAGE_MAP_TOOL_MAX_RECENT,
  PROP_TRUST_DIRTY_FLAG,
  PROP_SAVE_DEVICE_STATUS,
  PROP_SAVE_SESSION_INFO,
  PROP_RESTORE_SESSION,
  PROP_SAVE_TOOL_OPTIONS,
  PROP_SHOW_TOOLTIPS,
  PROP_TEAROFF_MENUS,
  PROP_CAN_CHANGE_ACCELS,
  PROP_SAVE_ACCELS,
  PROP_RESTORE_ACCELS,
  PROP_LAST_OPENED_SIZE,
  PROP_MAX_NEW_IMAGE_SIZE,
  PROP_TOOLBOX_COLOR_AREA,
  PROP_TOOLBOX_FOO_AREA,
  PROP_TOOLBOX_IMAGE_AREA,
  PROP_TOOLBOX_WILBER,
  PROP_THEME_PATH,
  PROP_THEME,
  PROP_USE_HELP,
  PROP_SHOW_HELP_BUTTON,
  PROP_HELP_LOCALES,
  PROP_HELP_BROWSER,
  PROP_USER_MANUAL_ONLINE,
  PROP_USER_MANUAL_ONLINE_URI,
  PROP_DOCK_WINDOW_HINT,
  PROP_CURSOR_FORMAT,
  PROP_CURSOR_HANDEDNESS,

  PROP_HIDE_DOCKS,
  PROP_SINGLE_WINDOW_MODE,
  PROP_LAST_TIP_SHOWN,

  /* ignored, only for backward compatibility: */
  PROP_INFO_WINDOW_PER_DISPLAY,
  PROP_MENU_MNEMONICS,
  PROP_SHOW_TOOL_TIPS,
  PROP_SHOW_TIPS,
  PROP_TOOLBOX_WINDOW_HINT,
  PROP_TRANSIENT_DOCKS,
  PROP_WEB_BROWSER
};


static void   gimp_gui_config_finalize     (GObject      *object);
static void   gimp_gui_config_set_property (GObject      *object,
                                            guint         property_id,
                                            const GValue *value,
                                            GParamSpec   *pspec);
static void   gimp_gui_config_get_property (GObject      *object,
                                            guint         property_id,
                                            GValue       *value,
                                            GParamSpec   *pspec);


G_DEFINE_TYPE (GimpGuiConfig, gimp_gui_config, GIMP_TYPE_DISPLAY_CONFIG)

#define parent_class gimp_gui_config_parent_class


static void
gimp_gui_config_class_init (GimpGuiConfigClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  gchar        *path;

  object_class->finalize     = gimp_gui_config_finalize;
  object_class->set_property = gimp_gui_config_set_property;
  object_class->get_property = gimp_gui_config_get_property;

  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_MOVE_TOOL_CHANGES_ACTIVE,
                                    "move-tool-changes-active",
                                    MOVE_TOOL_CHANGES_ACTIVE_BLURB,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_INT (object_class, PROP_IMAGE_MAP_TOOL_MAX_RECENT,
                                "image-map-tool-max-recent",
                                IMAGE_MAP_TOOL_MAX_RECENT_BLURB,
                                0, 255, 10,
                                GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_TRUST_DIRTY_FLAG,
                                    "trust-dirty-flag",
                                    TRUST_DIRTY_FLAG_BLURB,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_SAVE_DEVICE_STATUS,
                                    "save-device-status",
                                    SAVE_DEVICE_STATUS_BLURB,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_SAVE_SESSION_INFO,
                                    "save-session-info",
                                    SAVE_SESSION_INFO_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_RESTORE_SESSION,
                                    "restore-session", RESTORE_SESSION_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_SAVE_TOOL_OPTIONS,
                                    "save-tool-options",
                                    SAVE_TOOL_OPTIONS_BLURB,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_SHOW_TOOLTIPS,
                                    "show-tooltips", SHOW_TOOLTIPS_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS |
                                    GIMP_CONFIG_PARAM_RESTART);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_TEAROFF_MENUS,
                                    "tearoff-menus", TEAROFF_MENUS_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_CAN_CHANGE_ACCELS,
                                    "can-change-accels", CAN_CHANGE_ACCELS_BLURB,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_SAVE_ACCELS,
                                    "save-accels", SAVE_ACCELS_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_RESTORE_ACCELS,
                                    "restore-accels", RESTORE_ACCELS_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_INT (object_class, PROP_LAST_OPENED_SIZE,
                                "last-opened-size", LAST_OPENED_SIZE_BLURB,
                                0, 1024, 10,
                                GIMP_PARAM_STATIC_STRINGS |
                                GIMP_CONFIG_PARAM_RESTART);
  GIMP_CONFIG_INSTALL_PROP_MEMSIZE (object_class, PROP_MAX_NEW_IMAGE_SIZE,
                                    "max-new-image-size",
                                    MAX_NEW_IMAGE_SIZE_BLURB,
                                    0, GIMP_MAX_MEMSIZE, 1 << 27, /* 128MB */
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_TOOLBOX_COLOR_AREA,
                                    "toolbox-color-area",
                                    TOOLBOX_COLOR_AREA_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_TOOLBOX_FOO_AREA,
                                    "toolbox-foo-area",
                                    TOOLBOX_FOO_AREA_BLURB,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_TOOLBOX_IMAGE_AREA,
                                    "toolbox-image-area",
                                    TOOLBOX_IMAGE_AREA_BLURB,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_TOOLBOX_WILBER,
                                    "toolbox-wilber",
                                    TOOLBOX_WILBER_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  path = gimp_config_build_data_path ("themes");
  GIMP_CONFIG_INSTALL_PROP_PATH (object_class, PROP_THEME_PATH,
                                 "theme-path", THEME_PATH_BLURB,
                                 GIMP_CONFIG_PATH_DIR_LIST, path,
                                 GIMP_PARAM_STATIC_STRINGS |
                                 GIMP_CONFIG_PARAM_RESTART);
  g_free (path);
  GIMP_CONFIG_INSTALL_PROP_STRING (object_class, PROP_THEME,
                                   "theme", THEME_BLURB,
                                   DEFAULT_THEME,
                                   GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_USE_HELP,
                                    "use-help", USE_HELP_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_SHOW_HELP_BUTTON,
                                    "show-help-button", SHOW_HELP_BUTTON_BLURB,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_STRING (object_class, PROP_HELP_LOCALES,
                                   "help-locales", HELP_LOCALES_BLURB,
                                   "",
                                   GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_ENUM (object_class, PROP_HELP_BROWSER,
                                 "help-browser", HELP_BROWSER_BLURB,
                                 GIMP_TYPE_HELP_BROWSER_TYPE,
                                 DEFAULT_HELP_BROWSER,
                                 GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_USER_MANUAL_ONLINE,
                                    "user-manual-online",
                                    USER_MANUAL_ONLINE_BLURB,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_STRING (object_class, PROP_USER_MANUAL_ONLINE_URI,
                                   "user-manual-online-uri",
                                   USER_MANUAL_ONLINE_URI_BLURB,
                                   DEFAULT_USER_MANUAL_ONLINE_URI,
                                   GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_ENUM (object_class, PROP_DOCK_WINDOW_HINT,
                                 "dock-window-hint",
                                 DOCK_WINDOW_HINT_BLURB,
                                 GIMP_TYPE_WINDOW_HINT,
                                 GIMP_WINDOW_HINT_UTILITY,
                                 GIMP_PARAM_STATIC_STRINGS |
                                 GIMP_CONFIG_PARAM_RESTART);
  GIMP_CONFIG_INSTALL_PROP_ENUM (object_class, PROP_CURSOR_FORMAT,
                                 "cursor-format", CURSOR_FORMAT_BLURB,
                                 GIMP_TYPE_CURSOR_FORMAT,
                                 GIMP_CURSOR_FORMAT_PIXBUF,
                                 GIMP_PARAM_STATIC_STRINGS);
  GIMP_CONFIG_INSTALL_PROP_ENUM (object_class, PROP_CURSOR_HANDEDNESS,
                                 "cursor-handedness", CURSOR_HANDEDNESS_BLURB,
                                 GIMP_TYPE_HANDEDNESS,
                                 GIMP_HANDEDNESS_RIGHT,
                                 GIMP_PARAM_STATIC_STRINGS);

  g_object_class_install_property (object_class, PROP_HIDE_DOCKS,
                                   g_param_spec_boolean ("hide-docks",
                                                         NULL,
                                                         HIDE_DOCKS_BLURB,
                                                         FALSE,
                                                         G_PARAM_READWRITE |
                                                         G_PARAM_CONSTRUCT |
                                                         GIMP_PARAM_STATIC_STRINGS));
  g_object_class_install_property (object_class, PROP_SINGLE_WINDOW_MODE,
                                   g_param_spec_boolean ("single-window-mode",
                                                         NULL,
                                                         SINGLE_WINDOW_MODE_BLURB,
                                                         FALSE,
                                                         G_PARAM_READWRITE |
                                                         G_PARAM_CONSTRUCT |
                                                         GIMP_PARAM_STATIC_STRINGS));
  g_object_class_install_property (object_class, PROP_LAST_TIP_SHOWN,
                                   g_param_spec_int ("last-tip-shown",
                                                     NULL, NULL,
                                                     0, G_MAXINT, 0,
                                                     G_PARAM_READWRITE |
                                                     G_PARAM_CONSTRUCT |
                                                     GIMP_PARAM_STATIC_STRINGS));

  /*  only for backward compatibility:  */
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_INFO_WINDOW_PER_DISPLAY,
                                    "info-window-per-display",
                                    NULL,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS |
                                    GIMP_CONFIG_PARAM_IGNORE);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_MENU_MNEMONICS,
                                    "menu-mnemonics", NULL,
                                    TRUE,
                                    GIMP_PARAM_STATIC_STRINGS |
                                    GIMP_CONFIG_PARAM_IGNORE);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_SHOW_TOOL_TIPS,
                                    "show-tool-tips", NULL,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS |
                                    GIMP_CONFIG_PARAM_IGNORE);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_SHOW_TIPS,
                                    "show-tips", NULL,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS |
                                    GIMP_CONFIG_PARAM_IGNORE);
  GIMP_CONFIG_INSTALL_PROP_ENUM (object_class, PROP_TOOLBOX_WINDOW_HINT,
                                 "toolbox-window-hint", NULL,
                                 GIMP_TYPE_WINDOW_HINT,
                                 GIMP_WINDOW_HINT_UTILITY,
                                 GIMP_PARAM_STATIC_STRINGS |
                                 GIMP_CONFIG_PARAM_IGNORE);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_TRANSIENT_DOCKS,
                                    "transient-docks", NULL,
                                    FALSE,
                                    GIMP_PARAM_STATIC_STRINGS |
                                    GIMP_CONFIG_PARAM_IGNORE);
  GIMP_CONFIG_INSTALL_PROP_PATH (object_class, PROP_WEB_BROWSER,
                                 "web-browser", NULL,
                                 GIMP_CONFIG_PATH_FILE,
                                 "not used any longer",
                                 GIMP_PARAM_STATIC_STRINGS |
                                 GIMP_CONFIG_PARAM_IGNORE);
}

static void
gimp_gui_config_init (GimpGuiConfig *config)
{
}

static void
gimp_gui_config_finalize (GObject *object)
{
  GimpGuiConfig *gui_config = GIMP_GUI_CONFIG (object);

  g_free (gui_config->theme_path);
  g_free (gui_config->theme);
  g_free (gui_config->help_locales);
  g_free (gui_config->web_browser);
  g_free (gui_config->user_manual_online_uri);

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_gui_config_set_property (GObject      *object,
                              guint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  GimpGuiConfig *gui_config = GIMP_GUI_CONFIG (object);

  switch (property_id)
    {
    case PROP_MOVE_TOOL_CHANGES_ACTIVE:
      gui_config->move_tool_changes_active = g_value_get_boolean (value);
      break;
    case PROP_IMAGE_MAP_TOOL_MAX_RECENT:
      gui_config->image_map_tool_max_recent = g_value_get_int (value);
      break;
    case PROP_TRUST_DIRTY_FLAG:
      gui_config->trust_dirty_flag = g_value_get_boolean (value);
      break;
    case PROP_SAVE_DEVICE_STATUS:
      gui_config->save_device_status = g_value_get_boolean (value);
      break;
    case PROP_SAVE_SESSION_INFO:
      gui_config->save_session_info = g_value_get_boolean (value);
      break;
    case PROP_RESTORE_SESSION:
      gui_config->restore_session = g_value_get_boolean (value);
      break;
    case PROP_SAVE_TOOL_OPTIONS:
      gui_config->save_tool_options = g_value_get_boolean (value);
      break;
    case PROP_SHOW_TOOLTIPS:
      gui_config->show_tooltips = g_value_get_boolean (value);
      break;
    case PROP_TEAROFF_MENUS:
      gui_config->tearoff_menus = g_value_get_boolean (value);
      break;
    case PROP_CAN_CHANGE_ACCELS:
      gui_config->can_change_accels = g_value_get_boolean (value);
      break;
    case PROP_SAVE_ACCELS:
      gui_config->save_accels = g_value_get_boolean (value);
      break;
    case PROP_RESTORE_ACCELS:
      gui_config->restore_accels = g_value_get_boolean (value);
      break;
    case PROP_LAST_OPENED_SIZE:
      gui_config->last_opened_size = g_value_get_int (value);
      break;
    case PROP_MAX_NEW_IMAGE_SIZE:
      gui_config->max_new_image_size = g_value_get_uint64 (value);
      break;
    case PROP_TOOLBOX_COLOR_AREA:
      gui_config->toolbox_color_area = g_value_get_boolean (value);
      break;
    case PROP_TOOLBOX_FOO_AREA:
      gui_config->toolbox_foo_area = g_value_get_boolean (value);
      break;
    case PROP_TOOLBOX_IMAGE_AREA:
      gui_config->toolbox_image_area = g_value_get_boolean (value);
      break;
    case PROP_TOOLBOX_WILBER:
      gui_config->toolbox_wilber = g_value_get_boolean (value);
      break;
     case PROP_THEME_PATH:
      g_free (gui_config->theme_path);
      gui_config->theme_path = g_value_dup_string (value);
      break;
    case PROP_THEME:
      g_free (gui_config->theme);
      gui_config->theme = g_value_dup_string (value);
      break;
    case PROP_USE_HELP:
      gui_config->use_help = g_value_get_boolean (value);
      break;
    case PROP_SHOW_HELP_BUTTON:
      gui_config->show_help_button = g_value_get_boolean (value);
      break;
    case PROP_HELP_LOCALES:
      g_free (gui_config->help_locales);
      gui_config->help_locales = g_value_dup_string (value);
      break;
    case PROP_HELP_BROWSER:
      gui_config->help_browser = g_value_get_enum (value);
      break;
    case PROP_USER_MANUAL_ONLINE:
      gui_config->user_manual_online = g_value_get_boolean (value);
      break;
    case PROP_USER_MANUAL_ONLINE_URI:
      g_free (gui_config->user_manual_online_uri);
      gui_config->user_manual_online_uri = g_value_dup_string (value);
      break;
    case PROP_DOCK_WINDOW_HINT:
      gui_config->dock_window_hint = g_value_get_enum (value);
      break;
    case PROP_CURSOR_FORMAT:
      gui_config->cursor_format = g_value_get_enum (value);
      break;
    case PROP_CURSOR_HANDEDNESS:
      gui_config->cursor_handedness = g_value_get_enum (value);
      break;

    case PROP_HIDE_DOCKS:
      gui_config->hide_docks = g_value_get_boolean (value);
      break;
    case PROP_SINGLE_WINDOW_MODE:
      gui_config->single_window_mode = g_value_get_boolean (value);
      break;
    case PROP_LAST_TIP_SHOWN:
      gui_config->last_tip_shown = g_value_get_int (value);
      break;

    case PROP_INFO_WINDOW_PER_DISPLAY:
    case PROP_MENU_MNEMONICS:
    case PROP_SHOW_TOOL_TIPS:
    case PROP_SHOW_TIPS:
    case PROP_TOOLBOX_WINDOW_HINT:
    case PROP_TRANSIENT_DOCKS:
    case PROP_WEB_BROWSER:
      /* ignored */
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_gui_config_get_property (GObject    *object,
                              guint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  GimpGuiConfig *gui_config = GIMP_GUI_CONFIG (object);

  switch (property_id)
    {
    case PROP_MOVE_TOOL_CHANGES_ACTIVE:
      g_value_set_boolean (value, gui_config->move_tool_changes_active);
      break;
    case PROP_IMAGE_MAP_TOOL_MAX_RECENT:
      g_value_set_int (value, gui_config->image_map_tool_max_recent);
      break;
    case PROP_TRUST_DIRTY_FLAG:
      g_value_set_boolean (value, gui_config->trust_dirty_flag);
      break;
    case PROP_SAVE_DEVICE_STATUS:
      g_value_set_boolean (value, gui_config->save_device_status);
      break;
    case PROP_SAVE_SESSION_INFO:
      g_value_set_boolean (value, gui_config->save_session_info);
      break;
    case PROP_RESTORE_SESSION:
      g_value_set_boolean (value, gui_config->restore_session);
      break;
    case PROP_SAVE_TOOL_OPTIONS:
      g_value_set_boolean (value, gui_config->save_tool_options);
      break;
    case PROP_SHOW_TOOLTIPS:
      g_value_set_boolean (value, gui_config->show_tooltips);
      break;
    case PROP_TEAROFF_MENUS:
      g_value_set_boolean (value, gui_config->tearoff_menus);
      break;
    case PROP_CAN_CHANGE_ACCELS:
      g_value_set_boolean (value, gui_config->can_change_accels);
      break;
    case PROP_SAVE_ACCELS:
      g_value_set_boolean (value, gui_config->save_accels);
      break;
    case PROP_RESTORE_ACCELS:
      g_value_set_boolean (value, gui_config->restore_accels);
      break;
    case PROP_LAST_OPENED_SIZE:
      g_value_set_int (value, gui_config->last_opened_size);
      break;
    case PROP_MAX_NEW_IMAGE_SIZE:
      g_value_set_uint64 (value, gui_config->max_new_image_size);
      break;
    case PROP_TOOLBOX_COLOR_AREA:
      g_value_set_boolean (value, gui_config->toolbox_color_area);
      break;
    case PROP_TOOLBOX_FOO_AREA:
      g_value_set_boolean (value, gui_config->toolbox_foo_area);
      break;
    case PROP_TOOLBOX_IMAGE_AREA:
      g_value_set_boolean (value, gui_config->toolbox_image_area);
      break;
    case PROP_TOOLBOX_WILBER:
      g_value_set_boolean (value, gui_config->toolbox_wilber);
      break;
    case PROP_THEME_PATH:
      g_value_set_string (value, gui_config->theme_path);
      break;
    case PROP_THEME:
      g_value_set_string (value, gui_config->theme);
      break;
    case PROP_USE_HELP:
      g_value_set_boolean (value, gui_config->use_help);
      break;
    case PROP_SHOW_HELP_BUTTON:
      g_value_set_boolean (value, gui_config->show_help_button);
      break;
    case PROP_HELP_LOCALES:
      g_value_set_string (value, gui_config->help_locales);
      break;
    case PROP_HELP_BROWSER:
      g_value_set_enum (value, gui_config->help_browser);
      break;
    case PROP_USER_MANUAL_ONLINE:
      g_value_set_boolean (value, gui_config->user_manual_online);
      break;
    case PROP_USER_MANUAL_ONLINE_URI:
      g_value_set_string (value, gui_config->user_manual_online_uri);
      break;
    case PROP_DOCK_WINDOW_HINT:
      g_value_set_enum (value, gui_config->dock_window_hint);
      break;
    case PROP_CURSOR_FORMAT:
      g_value_set_enum (value, gui_config->cursor_format);
      break;
    case PROP_CURSOR_HANDEDNESS:
      g_value_set_enum (value, gui_config->cursor_handedness);
      break;

    case PROP_HIDE_DOCKS:
      g_value_set_boolean (value, gui_config->hide_docks);
      break;
    case PROP_SINGLE_WINDOW_MODE:
      g_value_set_boolean (value, gui_config->single_window_mode);
      break;
    case PROP_LAST_TIP_SHOWN:
      g_value_set_int (value, gui_config->last_tip_shown);
      break;

    case PROP_INFO_WINDOW_PER_DISPLAY:
    case PROP_MENU_MNEMONICS:
    case PROP_SHOW_TOOL_TIPS:
    case PROP_SHOW_TIPS:
    case PROP_TOOLBOX_WINDOW_HINT:
    case PROP_TRANSIENT_DOCKS:
    case PROP_WEB_BROWSER:
      /* ignored */
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
