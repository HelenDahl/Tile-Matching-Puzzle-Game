#ifndef CONFIG_H
#define CONFIG_H

/**************    MAIN MENU    **************/
//Window
#define MENU_BACKGROUND_PATH ":/res/bg_menu.jpg"
#define MENU_ICON ":/res/menu_logo.icns"
#define MENU_WIDTH 1200
#define MENU_HEIGHT 675
#define MENU_TITLE "QLink Menu"
#define MENU_WIDGETS_OFFSET 70
#define MENU_SPACING 15
//Title
#define MENU_TITLE_COLOR "#016075"
#define MENU_TITLE_FONTSIZE 45
//Button
#define MENU_BUTTON_FONTSIZE 15
#define MENU_BUTTON_WIDTH 250
#define MENU_BUTTON_HEIGHT 50
#define MENU_BUTTON_COLOR "#6C6C6D"
#define MENU_BUTTON_HOVER_COLOR "#8D8D8C"
#define MENU_BUTTON_CLICK_COLOR "#525352"

/**************    SETUP MENU    **************/
//Window
#define CONFIG_TITLE "User Config"
#define CONFIG_WIDTH 200
#define CONFIG_HEIGHT 300
//Titile
#define CONFIG_TITLE_LABEL_FONTSIZE 30
#define CONFIG_TITLE_LABEL_COLOR "#000000"
//Upper and lower limit
#define MAX_ROWS 8
#define MIN_ROWS 3
#define MAX_COLUMNS 10
#define MIN_COLUMNS 3
#define MAX_TIMELIMIT 6000
#define MIN_TIMELIMIT 10
//default
#define DEFAULT_ROWS 6
#define DEFAULT_COLUMNS 6
#define DEFAULT_TIMELIMIT 60
//Character interface
#define CHARACTER_LABEL_SIDELENGTH 80
#define CHARACTER_LABEL_TEXT_WIDTH 30
#define CHARACTER_LABEL_TEXT_HEIGHT 20
#define CHARACTER_LABEL_TEXT_FONTSIZE 15
#define ROLE1_PATH ":/res/role1_forward_idle.png"
#define ROLE2_PATH ":/res/role2_forward_idle.png"

/**************    SINGLE-PLAYER GAME INTERFACE    **************/
#define SINGLE_GAME_BG_PATH ":/res/bg_game.jpg"
#define SINGLE_GAME_ICON ":/res/game_logo.icns"
#define SINGLE_GAME_WIDTH 900
#define SINGLE_GAME_HEIGHT 750
#define SINGLE_GAME_BOX_WIDTH_RATIO 0.8
#define SINGLE_GAME_BOX_HEIGHT_RATIO 0.8
#define SINGLE_GAME_TIMER_FONTSIZE 50
#define SINGLE_GAME_SCORE_FONTSIZE 50

/**************    DOUBLE-PLAYER GAME INTERFACE    **************/
#define DOUBLE_GAME_BG_PATH ":/res/bg_game.jpg"
#define DOUBLE_GAME_ICON ":/res/game_logo.icns"
#define DOUBLE_GAME_WIDTH 900
#define DOUBLE_GAME_HEIGHT 750
#define DOUBLE_GAME_BOX_WIDTH_RATIO 0.9
#define DOUBLE_GAME_BOX_HEIGHT_RATIO 0.8
#define DOUBLE_GAME_TIMER_FONTSIZE 25
#define DOUBLE_GAME_SCORE_FONTSIZE 25
#define DOUBLE_GAME_TIME_X 40
#define DOUBLE_GAME_TIME_Y 80
#define DOUBLE_GAME_PLAYER_SCORE1_X 250
#define DOUBLE_GAME_PLAYER_SCORE1_Y 80
#define DOUBLE_GAME_PLAYER_SCORE2_X 500
#define DOUBLE_GAME_PLAYER_SCORE2_Y 80


/**************    GAME INTERFACE    **************/
#define GAME_BACKGROUND_PATH ":/res/bg_game.jpg"
#define GAME_WIDTH 1024
#define GAME_HEIGHT 987
#define GAME_TITLE_SINGLE "QLink--Single Mode"
#define GAME_TITLE_DOUBLE "QLink--Double Mode"
#define GAME_ICON ":/res/game_logo.icns"
#define GAME_COUNTDOWN_ICON ":/res/countDownIcon.png"
#define GAME_PAUSE_ICON ":/res/pauseIcon.png"
#define GAME_PAUSE_HOVER_ICON ":/res/pauseIcon_hover.png"
#define GAME_PAUSE_CLICK_ICON ":/res/pauseIcon_click.png"
#define GAME_HAVE_PAUSED_ICON ":/res/havePausedIcon.png"
#define GAME_PAUSE_ICON_SIDELENGTH 50


/**************    BLOCK    **************/
#define BLOCK_WIDTH 50
#define BLOCK_HEIGHT 50
#define BLOCK_PATH ":/res/block-%1.png"
#define BLOCK_CHOSEN_BORDER 4
#define MAX_BLOCK_TYPES 12
#define BLOCK_SCORE 1

/**************    GAME LOGIC    **************/
#define GAME_RATE 10
//Time remaining after deleting two connected blocks
#define BLOCK_REMAINTIME 20
//Tool icons
#define EXTENDTIME_ICON ":/res/extendTime.png"
#define SHUFFLE_ICON ":/res/shuffle.png"
#define HINT_ICON ":/res/hint.png"
#define FLASH_ICON ":/res/flash.png"
#define FREEZE_ICON ":/res/freeze.png"
#define DIZZY_ICON ":/res/dizzy.png"
//Tool size
#define TOOLS_SIZE 20
#define TOOLS_ELIPSIZE 30
//Interval time for tools appearance
#define TOOLS_INTERVAL 700
//Maximum number of tools
#define TOOLS_MAX_NUM_SINGLE 10
#define TOOLS_MAX_NUM_DOUBLE 15

/**************    PAUSE MENU    **************/
#define GAMEPAUSE_TITLE "Pause Menu"
#define GAMEPAUSE_TITLE_COLOR "#6B7F26"
#define GAMEPAUSE_TITLE_FONTSIZE 60
#define GAMEPAUSE_BUTTON_COLOR "#2382AB"
#define GAMEPAUSE_BUTTON_HOVER_COLOR "#3E94BA"
#define GAMEPAUSE_BUTTON_CLICK_COLOR "#22607C"
#define GAMEPAUSE_BUTTON_WIDTH 350
#define GAMEPAUSE_BUTTON_HEIGHT 50
#define GAMEPAUSE_BUTTON_FONTSIZE 30

/**************    GAME OVER INTERFACE    **************/
#define GAMEOVER_WIDTH 500
#define GAMEOVER_HEIGHT 400
#define GAMEOVER_TITLE "Game Over"
#define GAMEOVER_LOGO_PATH ":/res/gameOver.png"
#define GAMEOVER_LOGO_WIDTH 200
#define GAMEOVER_LOGO_HEIGHT 100
#define GAMEOVER_LABEL_FONTSIZE 20
#define GAMEOVER_BUTTON_COLOR "#46619C"
#define GAMEOVER_BUTTON_HOVERCOLOR "#526EA9"
#define GAMEOVER_BUTTON_CLICKCOLOR "#3B5999"
#define GAMEOVER_BUTTON_FONTSIZE 20
#define GAMEOVER_BUTTON_WIDTH 200
#define GAMEOVER_BUTTON_HEIGHT 35
#define GAMEOVER_LAYOUT_SPACING 10

#define PLAYER_SPEED 1
#define PLAYER_MAX_PIC 2

/**************    PLAYER 1    **************/
#define PLAYER_1_FORWARD_IDLE_PATH ":/res/role1_forward_idle.png"
#define PLAYER_1_BACKWARD_IDLE_PATH ":/res/role1_backward_idle.png"
#define PLAYER_1_LEFTWARD_IDLE_PATH ":/res/role1_leftward_idle.png"
#define PLAYER_1_RIGHTWARD_IDLE_PATH ":/res/role1_rightward_idle.png"
#define PLAYER_1_FORWARD_PATH ":/res/role1_forward-%1.png"
#define PLAYER_1_BACKWARD_PATH ":/res/role1_backward-%1.png"
#define PLAYER_1_LEFTWARD_PATH ":/res/role1_leftward-%1.png"
#define PLAYER_1_RIGHTWARD_PATH ":/res/role1_rightward-%1.png"
#define PLAYER_1_PIXMAPRECORDER_MAX 20
#define PLAYER_1_SIDELENGTH 30

/**************    PLAYER 2    **************/
#define PLAYER_2_FORWARD_IDLE_PATH ":/res/role2_forward_idle.png"
#define PLAYER_2_BACKWARD_IDLE_PATH ":/res/role2_backward_idle.png"
#define PLAYER_2_LEFTWARD_IDLE_PATH ":/res/role2_leftward_idle.png"
#define PLAYER_2_RIGHTWARD_IDLE_PATH ":/res/role2_rightward_idle.png"
#define PLAYER_2_FORWARD_PATH ":/res/role2_forward-%1.png"
#define PLAYER_2_BACKWARD_PATH ":/res/role2_backward-%1.png"
#define PLAYER_2_LEFTWARD_PATH ":/res/role2_leftward-%1.png"
#define PLAYER_2_RIGHTWARD_PATH ":/res/role2_rightward-%1.png"
#define PLAYER_2_PIXMAPRECORDER_MAX 20
#define PLAYER_2_SIDELENGTH 30

#endif // CONFIG_H
