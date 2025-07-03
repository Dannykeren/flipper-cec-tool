#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/popup.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <expansion/expansion.h>

#define TAG "CECTestTool"

typedef enum {
    CECTestToolViewSubmenu,
    CECTestToolViewTextInput,
    CECTestToolViewPopup,
    CECTestToolViewDialog,
    CECTestToolViewWidget,
} CECTestToolView;

typedef enum {
    CECTestToolSubmenuIndexSendCommand,
    CECTestToolSubmenuIndexReceiveCommand,
    CECTestToolSubmenuIndexDeviceDiscovery,
    CECTestToolSubmenuIndexPowerControl,
    CECTestToolSubmenuIndexVolumeControl,
    CECTestToolSubmenuIndexLogViewer,
    CECTestToolSubmenuIndexSettings,
} CECTestToolSubmenuIndex;

typedef struct {
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    TextInput* text_input;
    Popup* popup;
    DialogEx* dialog;
    Widget* widget;
    NotificationApp* notifications;
    
    // CEC protocol variables
    char text_input_buffer[32];
    char command_buffer[64];
    char log_buffer[512];
    uint8_t cec_address;
    bool is_connected;
    bool is_receiving;
    
    // Game module connection
    Expansion* expansion;
    
    // CEC command history
    char command_history[10][64];
    int command_count;
} CECTestTool;

// CEC Protocol Constants
#define CEC_BROADCAST_ADDRESS 0x0F
#define CEC_TV_ADDRESS 0x00
#define CEC_PLAYBACK_1_ADDRESS 0x04
#define CEC_AUDIO_SYSTEM_ADDRESS 0x05

// CEC Command Opcodes
#define CEC_OPCODE_ACTIVE_SOURCE 0x82
#define CEC_OPCODE_INACTIVE_SOURCE 0x9D
#define CEC_OPCODE_GIVE_DEVICE_POWER_STATUS 0x8F
#define CEC_OPCODE_REPORT_POWER_STATUS 0x90
#define CEC_OPCODE_GIVE_OSD_NAME 0x46
#define CEC_OPCODE_SET_OSD_NAME 0x47
#define CEC_OPCODE_STANDBY 0x36
#define CEC_OPCODE_USER_CONTROL_PRESSED 0x44
#define CEC_OPCODE_USER_CONTROL_RELEASED 0x45
#define CEC_OPCODE_GIVE_DEVICE_VENDOR_ID 0x8C
#define CEC_OPCODE_DEVICE_VENDOR_ID 0x87

// Forward declarations
static void cec_test_tool_submenu_callback(void* context, uint32_t index);
static void cec_test_tool_text_input_callback(void* context);
static void cec_test_tool_popup_callback(void* context);
static void cec_test_tool_dialog_callback(DialogExResult result, void* context);
static uint32_t cec_test_tool_navigation_callback(void* context);

// CEC Protocol Functions
static bool cec_send_command(CECTestTool* app, uint8_t src_addr, uint8_t dst_addr, uint8_t opcode, uint8_t* params, uint8_t param_len) {
    if(!app->is_connected) {
        FURI_LOG_E(TAG, "Game module not connected");
        return false;
    }
    
    // Build CEC frame
    uint8_t frame[16];
    frame[0] = (src_addr << 4) | dst_addr;  // Header byte
    frame[1] = opcode;
    
    for(int i = 0; i < param_len && i < 14; i++) {
        frame[2 + i] = params[i];
    }
    
    // Send via expansion module
    // This would interface with the game module's HDMI CEC functionality
    FURI_LOG_I(TAG, "Sending CEC command: 0x%02X to address 0x%02X", opcode, dst_addr);
    
    // Log the command
    snprintf(app->command_history[app->command_count % 10], 64, 
             "SENT: [%02X->%02X] %02X", src_addr, dst_addr, opcode);
    app->command_count++;
    
    return true;
}

static bool cec_receive_command(CECTestTool* app) {
    if(!app->is_connected) {
        return false;
    }
    
    // This would read from the game module's CEC receive buffer
    // For now, simulate receiving a command
    static uint8_t sim_counter = 0;
    
    if(sim_counter % 100 == 0) {  // Simulate occasional messages
        snprintf(app->command_history[app->command_count % 10], 64, 
                 "RECV: [%02X->%02X] %02X", CEC_TV_ADDRESS, CEC_PLAYBACK_1_ADDRESS, CEC_OPCODE_GIVE_DEVICE_POWER_STATUS);
        app->command_count++;
        return true;
    }
    
    sim_counter++;
    return false;
}

static void cec_device_discovery(CECTestTool* app) {
    FURI_LOG_I(TAG, "Starting CEC device discovery");
    
    // Send polling messages to all possible addresses
    for(uint8_t addr = 0; addr < 15; addr++) {
        if(addr == app->cec_address) continue;  // Skip our own address
        
        // Send a polling message (empty frame)
        cec_send_command(app, app->cec_address, addr, 0, NULL, 0);
        furi_delay_ms(100);
    }
    
    snprintf(app->command_history[app->command_count % 10], 64, 
             "Discovery: Polling all addresses");
    app->command_count++;
}

static void cec_power_control(CECTestTool* app, bool power_on) {
    uint8_t opcode = power_on ? CEC_OPCODE_ACTIVE_SOURCE : CEC_OPCODE_STANDBY;
    
    if(power_on) {
        // Send active source with physical address
        uint8_t params[2] = {0x10, 0x00};  // Physical address 1.0.0.0
        cec_send_command(app, app->cec_address, CEC_BROADCAST_ADDRESS, opcode, params, 2);
    } else {
        // Send standby to all devices
        cec_send_command(app, app->cec_address, CEC_BROADCAST_ADDRESS, opcode, NULL, 0);
    }
}

static void cec_volume_control(CECTestTool* app, bool volume_up) {
    uint8_t key_code = volume_up ? 0x41 : 0x42;  // Volume up/down
    uint8_t params[1] = {key_code};
    
    // Send user control pressed
    cec_send_command(app, app->cec_address, CEC_AUDIO_SYSTEM_ADDRESS, CEC_OPCODE_USER_CONTROL_PRESSED, params, 1);
    
    // Send user control released after delay
    furi_delay_ms(100);
    cec_send_command(app, app->cec_address, CEC_AUDIO_SYSTEM_ADDRESS, CEC_OPCODE_USER_CONTROL_RELEASED, NULL, 0);
}

// View callbacks
static void cec_test_tool_submenu_callback(void* context, uint32_t index) {
    CECTestTool* app = context;
    
    switch(index) {
        case CECTestToolSubmenuIndexSendCommand:
            view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewTextInput);
            break;
        case CECTestToolSubmenuIndexReceiveCommand:
            app->is_receiving = !app->is_receiving;
            popup_set_header(app->popup, app->is_receiving ? "Receiving CEC Commands" : "Stopped Receiving", 0, 0, AlignCenter, AlignTop);
            popup_set_text(app->popup, app->is_receiving ? "Monitoring CEC bus...\nPress back to stop" : "Press OK to start", 0, 20, AlignCenter, AlignTop);
            view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewPopup);
            break;
        case CECTestToolSubmenuIndexDeviceDiscovery:
            cec_device_discovery(app);
            popup_set_header(app->popup, "Device Discovery", 0, 0, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Scanning for CEC devices...\nCheck log for results", 0, 20, AlignCenter, AlignTop);
            view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewPopup);
            break;
        case CECTestToolSubmenuIndexPowerControl:
            dialog_ex_set_header(app->dialog, "Power Control", 64, 0, AlignCenter, AlignTop);
            dialog_ex_set_text(app->dialog, "Send power command?", 64, 20, AlignCenter, AlignTop);
            dialog_ex_set_left_button_text(app->dialog, "Power On");
            dialog_ex_set_right_button_text(app->dialog, "Standby");
            dialog_ex_set_center_button_text(app->dialog, "Cancel");
            view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewDialog);
            break;
        case CECTestToolSubmenuIndexVolumeControl:
            dialog_ex_set_header(app->dialog, "Volume Control", 64, 0, AlignCenter, AlignTop);
            dialog_ex_set_text(app->dialog, "Adjust volume?", 64, 20, AlignCenter, AlignTop);
            dialog_ex_set_left_button_text(app->dialog, "Vol Up");
            dialog_ex_set_right_button_text(app->dialog, "Vol Down");
            dialog_ex_set_center_button_text(app->dialog, "Cancel");
            view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewDialog);
            break;
        case CECTestToolSubmenuIndexLogViewer:
            // Build log display
            strcpy(app->log_buffer, "CEC Command Log:\n\n");
            for(int i = 0; i < 10 && i < app->command_count; i++) {
                int idx = (app->command_count - 1 - i) % 10;
                strcat(app->log_buffer, app->command_history[idx]);
                strcat(app->log_buffer, "\n");
            }
            
            widget_reset(app->widget);
            widget_add_text_scroll_element(app->widget, 0, 0, 128, 64, app->log_buffer);
            view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewWidget);
            break;
        case CECTestToolSubmenuIndexSettings:
            text_input_set_header_text(app->text_input, "Set CEC Address (0-14)");
            text_input_set_result_callback(app->text_input, cec_test_tool_text_input_callback, app, app->text_input_buffer, sizeof(app->text_input_buffer), true);
            snprintf(app->text_input_buffer, sizeof(app->text_input_buffer), "%d", app->cec_address);
            view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewTextInput);
            break;
        default:
            break;
    }
}

static void cec_test_tool_text_input_callback(void* context) {
    CECTestTool* app = context;
    
    // Handle different text input contexts
    int value = atoi(app->text_input_buffer);
    
    if(value >= 0 && value <= 14) {
        app->cec_address = value;
        FURI_LOG_I(TAG, "CEC address set to: %d", app->cec_address);
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewSubmenu);
}

static void cec_test_tool_popup_callback(void* context) {
    CECTestTool* app = context;
    view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewSubmenu);
}

static void cec_test_tool_dialog_callback(DialogExResult result, void* context) {
    CECTestTool* app = context;
    
    switch(result) {
        case DialogExResultLeft:
            // Power On / Volume Up
            if(strstr(dialog_ex_get_header(app->dialog), "Power")) {
                cec_power_control(app, true);
            } else {
                cec_volume_control(app, true);
            }
            break;
        case DialogExResultRight:
            // Standby / Volume Down
            if(strstr(dialog_ex_get_header(app->dialog), "Power")) {
                cec_power_control(app, false);
            } else {
                cec_volume_control(app, false);
            }
            break;
        case DialogExResultCenter:
        default:
            break;
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, CECTestToolViewSubmenu);
}

static uint32_t cec_test_tool_navigation_callback(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

static CECTestTool* cec_test_tool_alloc() {
    CECTestTool* app = malloc(sizeof(CECTestTool));
    
    // Initialize variables
    app->cec_address = CEC_PLAYBACK_1_ADDRESS;
    app->is_connected = false;
    app->is_receiving = false;
    app->command_count = 0;
    
    // Initialize GUI
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    
    // Submenu
    app->submenu = submenu_alloc();
    submenu_add_item(app->submenu, "Send Command", CECTestToolSubmenuIndexSendCommand, cec_test_tool_submenu_callback, app);
    submenu_add_item(app->submenu, "Receive Commands", CECTestToolSubmenuIndexReceiveCommand, cec_test_tool_submenu_callback, app);
    submenu_add_item(app->submenu, "Device Discovery", CECTestToolSubmenuIndexDeviceDiscovery, cec_test_tool_submenu_callback, app);
    submenu_add_item(app->submenu, "Power Control", CECTestToolSubmenuIndexPowerControl, cec_test_tool_submenu_callback, app);
    submenu_add_item(app->submenu, "Volume Control", CECTestToolSubmenuIndexVolumeControl, cec_test_tool_submenu_callback, app);
    submenu_add_item(app->submenu, "View Log", CECTestToolSubmenuIndexLogViewer, cec_test_tool_submenu_callback, app);
    submenu_add_item(app->submenu, "Settings", CECTestToolSubmenuIndexSettings, cec_test_tool_submenu_callback, app);
    
    view_set_previous_callback(submenu_get_view(app->submenu), cec_test_tool_navigation_callback);
    view_di
