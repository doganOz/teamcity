#include <gui/main_screen/MainView.hpp>
#include <touchgfx/Unicode.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

MainView::MainView():
    imageColorPickerClickedCallback(this, &MainView::imageColorPickerClickHandler),
    setTemperature(25),
    selectedMenu(MENU_NONE),
    topMenu(TOP_MENU_NONE),
    stateModeMenu(NOT_SHOWING),
    statePlanMenu(NOT_SHOWING),
    stateTimerMenu(NOT_SHOWING),
    stateSetupMenu(NOT_SHOWING),
    planDisplay(NOT_SHOWING),
    colorPickerScreen(NOT_SHOWING),
    idleScreen(NOT_SHOWING),
    selectedPlan(PLAN_NONE),
    selectedSetup(SETUP_NONE),
    menuCursorPosition(LEVEL_1),
    showMenuTickCounter(0),
    keyTimeOutCounter(0),
    goToIdleStateTimeOutCounter(0),
    planClicked(false),
    colorPickerClicked(false),
    NumberSetupMenu(0),
    setupMenuSetupClick(false),
    setupMenuTimerClick(false),
    setupMenuPlanClick(false),
    setupMenuModeClick(false),
    color_golden_yellow(touchgfx::Color::getColorFromRGB(250, 206, 39)),
    color_dark_red(touchgfx::Color::getColorFromRGB(150, 0, 4)),
    color_dirty_white(touchgfx::Color::getColorFromRGB(255, 255, 215)),
    color_orange(touchgfx::Color::getColorFromRGB(255, 189, 104))
{
    modeState.boostActive = false;
    modeState.ecoActive = false;
    modeState.silentActive = true;
    modeState.angleActive = false;

    timerState.timerActive = false;
    timerState.hours2Active = false;
    timerState.hours4Active = false;
    timerState.hours6Active = false;
}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();
    updateTimerIcon();
    updateBoostIcon();
    updateEcoIcon();
    updateSilentIcon();
    updateAngleIcon();
    imageColorPicker.setClickAction(imageColorPickerClickedCallback);
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}

void MainView::upPressed()
{
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;

    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && planDisplay == NOT_SHOWING
            && colorPickerScreen == NOT_SHOWING
            && idleScreen == NOT_SHOWING
       )
    {
        if (setTemperature < 40)
        {
            setTemperature++;
            Unicode::itoa(setTemperature, textAreaSetTemperatureBuffer, TEXTAREASETTEMPERATURE_SIZE, 10);
            Unicode::itoa(setTemperature, textAreaShadowSetTemperatureBuffer, TEXTAREASHADOWSETTEMPERATURE_SIZE, 10);
            containerSetTemperature.invalidate();
        }
    }
    else if (stateModeMenu == SHOWING
             || statePlanMenu == SHOWING
             || stateTimerMenu == SHOWING
             || stateSetupMenu == SHOWING
            )
    {
        switch (menuCursorPosition)
        {
        case LEVEL_1:
            menuCursorPosition = LEVEL_2;
            menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
            textAreaMenu2.setColor(color_dirty_white);
            if (stateModeMenu == SHOWING)
            {
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ACTIVE_ID));
                menuBackground2.invalidate();
                menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu1.setColor(color_dark_red);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ID));
            }
            else if (statePlanMenu == SHOWING)
            {
                selectedPlan = PLAN_WEEK;
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
                arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground2.invalidate();
                menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu1.setColor(color_dark_red);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
                arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            else if (stateTimerMenu == SHOWING)
            {
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
                menuBackground2.invalidate();
                menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu1.setColor(color_dark_red);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            }
            else if (stateSetupMenu == SHOWING)
            {
                selectedSetup = SETUP_TIMER_SETTINGS;
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ACTIVE_ID));
                arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground2.invalidate();
                menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu1.setColor(color_dark_red);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_COLOR_PICKER_ID));
                arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            menuBackground1.invalidate();
            break;
        case LEVEL_2:
            menuCursorPosition = LEVEL_3;
            menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
            textAreaMenu3.setColor(color_dirty_white);
            if (stateModeMenu == SHOWING)
            {
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ACTIVE_ID));
                menuBackground3.invalidate();
                menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu2.setColor(color_dark_red);
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ID));
            }
            else if (statePlanMenu == SHOWING)
            {
                selectedPlan = PLAN_MONTH;
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground3.invalidate();
                menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu2.setColor(color_dark_red);
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
                arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            else if (stateTimerMenu == SHOWING)
            {
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
                menuBackground3.invalidate();
                menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu2.setColor(color_dark_red);
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            }
            else if (stateSetupMenu == SHOWING)
            {
                selectedSetup = SETUP_THEMES;
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ACTIVE_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground3.invalidate();
                menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu2.setColor(color_dark_red);
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ID));
                arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            menuBackground2.invalidate();
            break;
        case LEVEL_3:
            menuCursorPosition = LEVEL_4;
            menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
            textAreaMenu4.setColor(color_dirty_white);
            if (stateModeMenu == SHOWING)
            {
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ACTIVE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu3.setColor(color_dark_red);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ID));
            }
            else if (statePlanMenu == SHOWING)
            {
                selectedPlan = PLAN_NONE;
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu3.setColor(color_dark_red);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            else if (stateTimerMenu == SHOWING)
            {
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu3.setColor(color_dark_red);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            }
            else if (stateSetupMenu == SHOWING)
            {
                selectedSetup = SETUP_CUSTOMIZE;
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu3.setColor(color_dark_red);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            menuBackground3.invalidate();
            break;
        case LEVEL_4:
            menuCursorPosition = LEVEL_DOWN_BUTTON;
            menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu4.setColor(color_dark_red);
            if (stateModeMenu == SHOWING)
            {
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            else if (statePlanMenu == SHOWING)
            {
                selectedPlan = PLAN_NONE;
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            else if (stateTimerMenu == SHOWING)
            {
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            else if (stateSetupMenu == SHOWING)
            {
                selectedSetup = SETUP_NONE;
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            menuBackground4.invalidate();
            menuDownIcon.setBitmap(Bitmap(BITMAP_ARROW_TAB_ACTIVE_ID));
            menuDownIcon.invalidate();
            break;
        case LEVEL_DOWN_BUTTON:
            menuDownIcon.setBitmap(Bitmap(BITMAP_ARROW_TAB_ACTIVE_ID));
            menuDownIcon.invalidate();
            //nop
            break;
        }
    }
}

void MainView::downPressed()
{
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;

    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && planDisplay == NOT_SHOWING
            && colorPickerScreen == NOT_SHOWING
            && idleScreen == NOT_SHOWING
       )
    {
        if (setTemperature > 0)
        {
            setTemperature--;
            Unicode::itoa(setTemperature, textAreaSetTemperatureBuffer, TEXTAREASETTEMPERATURE_SIZE, 10);
            Unicode::itoa(setTemperature, textAreaShadowSetTemperatureBuffer, TEXTAREASHADOWSETTEMPERATURE_SIZE, 10);
            containerSetTemperature.invalidate();
        }
    }
    else if (stateModeMenu == SHOWING
             || statePlanMenu == SHOWING
             || stateTimerMenu == SHOWING
             || stateSetupMenu == SHOWING
            )
    {
        switch (menuCursorPosition)
        {
        case LEVEL_1:
            //nop
            break;
        case LEVEL_2:
            menuCursorPosition = LEVEL_1;
            menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu2.setColor(color_dark_red);
            if (stateModeMenu == SHOWING)
            {
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ID));
                menuBackground2.invalidate();
                menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu1.setColor(color_dirty_white);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ACTIVE_ID));
            }
            else if (statePlanMenu == SHOWING)
            {
                selectedPlan = PLAN_DAY;
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
                arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                menuBackground2.invalidate();
                menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu1.setColor(color_dirty_white);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
                arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
            }
            else if (stateTimerMenu == SHOWING)
            {
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
                menuBackground2.invalidate();
                menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu1.setColor(color_dirty_white);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
            }
            else if (stateSetupMenu == SHOWING)
            {
                selectedSetup = SETUP_COLOR_PICKER;
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ID));
                arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                menuBackground2.invalidate();
                menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu1.setColor(color_dirty_white);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_COLOR_PICKER_ACTIVE_ID));
                arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
            }
            menuBackground1.invalidate();
            break;
        case LEVEL_3:
            menuCursorPosition = LEVEL_2;
            menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu3.setColor(color_dark_red);
            if (stateModeMenu == SHOWING)
            {
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ID));
                menuBackground3.invalidate();
                menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu2.setColor(color_dirty_white);
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ACTIVE_ID));
            }
            else if (statePlanMenu == SHOWING)
            {
                selectedPlan = PLAN_WEEK;
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                menuBackground3.invalidate();
                menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu2.setColor(color_dirty_white);
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
                arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
            }
            else if (stateTimerMenu == SHOWING)
            {
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
                menuBackground3.invalidate();
                menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu2.setColor(color_dirty_white);
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
            }
            else if (stateSetupMenu == SHOWING)
            {
                selectedSetup = SETUP_TIMER_SETTINGS;
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                menuBackground3.invalidate();
                menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu2.setColor(color_dirty_white);
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ACTIVE_ID));
                arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
            }
            menuBackground2.invalidate();
            break;
        case LEVEL_4:
            menuCursorPosition = LEVEL_3;
            menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu4.setColor(color_dark_red);
            if (stateModeMenu == SHOWING)
            {
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu3.setColor(color_dirty_white);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ACTIVE_ID));
            }
            else if (statePlanMenu == SHOWING)
            {
                selectedPlan = PLAN_MONTH;
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu3.setColor(color_dirty_white);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
            }
            else if (stateTimerMenu == SHOWING)
            {
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu3.setColor(color_dirty_white);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
            }
            else if (stateSetupMenu == SHOWING)
            {
                selectedSetup = SETUP_THEMES;
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                textAreaMenu3.setColor(color_dirty_white);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ACTIVE_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
            }
            menuBackground3.invalidate();
            break;
        case LEVEL_DOWN_BUTTON:
            menuCursorPosition = LEVEL_4;
            menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
            textAreaMenu4.setColor(color_dirty_white);
            if (stateModeMenu == SHOWING)
            {
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ACTIVE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu3.setColor(color_dark_red);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ID));
            }
            else if (statePlanMenu == SHOWING)
            {
                selectedPlan = PLAN_NONE;
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu3.setColor(color_dark_red);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            else if (stateTimerMenu == SHOWING)
            {
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu3.setColor(color_dark_red);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            }
            else if (stateSetupMenu == SHOWING)
            {
                selectedSetup = SETUP_CUSTOMIZE;
                iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
                arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                menuBackground4.invalidate();
                menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
                textAreaMenu3.setColor(color_dark_red);
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ID));
                arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            }
            menuBackground3.invalidate();
            menuDownIcon.setBitmap(Bitmap(BITMAP_ARROW_TAB_NORMAL_ID));
            menuDownIcon.invalidate();
            break;
        }
    }
}

void MainView::rightPressed()
{
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;

    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && planDisplay == NOT_SHOWING // You cannot select the bottom menu when showing a plan mode
            && colorPickerScreen == NOT_SHOWING // You cannot select the bottom menu when showing the color picker screen
            && idleScreen == NOT_SHOWING // You cannot select the bottom menu when showing the IDLE screen
       )
    {
        switch (selectedMenu)
        {
        case MENU_NONE:
            menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_MENU_SELECTED_ID));
            textAreaMode.setColor(color_dark_red);
            boxModeSelected.moveTo(0, 0);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();
            selectedMenu = MENU_MODE;
            break;
        case MENU_MODE:
            menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_NORMAL_ID));
            textAreaMode.setColor(color_golden_yellow);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();

            menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_MENU_SELECTED_ID));
            textAreaPlan.setColor(color_dark_red);
            boxModeSelected.moveTo(60, 0);
            boxModeSelected.invalidate();
            selectedMenu = MENU_PLAN;
            break;
        case MENU_PLAN:
            menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
            textAreaPlan.setColor(color_golden_yellow);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();

            menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_MENU_SELECTED_ID));
            textAreaTimer.setColor(color_dark_red);
            boxModeSelected.moveTo(120, 0);
            boxModeSelected.invalidate();
            selectedMenu = MENU_TIMER;
            break;
        case MENU_TIMER:
            menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_NORMAL_ID));
            textAreaTimer.setColor(color_golden_yellow);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();

            menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_MENU_SELECTED_ID));
            textAreaSetup.setColor(color_dark_red);
            boxModeSelected.moveTo(180, 0);
            boxModeSelected.invalidate();
            selectedMenu = MENU_SETUP;
            break;
        case MENU_SETUP:
            menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
            textAreaSetup.setColor(color_golden_yellow);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();

            menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_MENU_SELECTED_ID));
            textAreaMode.setColor(color_dark_red);
            boxModeSelected.moveTo(0, 0);
            boxModeSelected.invalidate();
            selectedMenu = MENU_MODE;
            break;
        }
    }
    else if (planDisplay == SHOWING || colorPickerScreen == SHOWING)
    {
        /* Navigate between Edit and Back button in top menu */
        switch (topMenu)
        {
        case TOP_MENU_NONE:
            /* When no element selected, when right pressed, go to Edit button */
            backgroundTopMenuButton.invalidate();
            backgroundTopMenuButton.moveTo(200, 0);
            backgroundTopMenuButton.setVisible(true);
            iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_ACTIVE_ID));
            backgroundTopMenuButton.invalidate();
            topMenu = TOP_MENU_EDIT;
            break;
        case TOP_MENU_BACK:
            /* When element back selected, and right pressed, go to Edit button */
            backgroundTopMenuButton.invalidate();
            iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_NORMAL_ID));
            iconBackTopMenu.invalidate();
            backgroundTopMenuButton.moveTo(200, 0);
            backgroundTopMenuButton.setVisible(true);
            iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_ACTIVE_ID));
            backgroundTopMenuButton.invalidate();
            topMenu = TOP_MENU_EDIT;
            break;
        case TOP_MENU_EDIT:
            /* When element back selected, and right pressed, do nothing */
            break;
        }
    }
}

void MainView::leftPressed()
{
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;

    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && planDisplay == NOT_SHOWING // You cannot select the bottom menu when showing a plan mode
            && colorPickerScreen == NOT_SHOWING // You cannot select the bottom menu when showing the color picker screen
            && idleScreen == NOT_SHOWING // You cannot select the bottom menu when showing the IDLE screen
       )
    {
        switch (selectedMenu)
        {
        case MENU_NONE:
            menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_MENU_SELECTED_ID));
            boxModeSelected.moveTo(180, 0);
            boxModeSelected.setVisible(true);
            textAreaSetup.setColor(color_dark_red);
            boxModeSelected.invalidate();
            selectedMenu = MENU_SETUP;
            break;
        case MENU_MODE:
            menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_NORMAL_ID));
            textAreaMode.setColor(color_golden_yellow);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();

            menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_MENU_SELECTED_ID));
            textAreaSetup.setColor(color_dark_red);
            boxModeSelected.moveTo(180, 0);
            boxModeSelected.invalidate();
            selectedMenu = MENU_SETUP;
            break;
        case MENU_PLAN:
            menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
            textAreaPlan.setColor(color_golden_yellow);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();

            menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_MENU_SELECTED_ID));
            textAreaMode.setColor(color_dark_red);
            boxModeSelected.moveTo(0, 0);
            boxModeSelected.invalidate();
            selectedMenu = MENU_MODE;
            break;
        case MENU_TIMER:
            menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_NORMAL_ID));
            textAreaTimer.setColor(color_golden_yellow);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();

            menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_MENU_SELECTED_ID));
            textAreaPlan.setColor(color_dark_red);
            boxModeSelected.moveTo(60, 0);
            boxModeSelected.invalidate();
            selectedMenu = MENU_PLAN;
            break;
        case MENU_SETUP:
            menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
            textAreaSetup.setColor(color_golden_yellow);
            boxModeSelected.setVisible(true);
            boxModeSelected.invalidate();

            menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_MENU_SELECTED_ID));
            textAreaTimer.setColor(color_dark_red);
            boxModeSelected.moveTo(120, 0);
            boxModeSelected.invalidate();
            selectedMenu = MENU_TIMER;
            break;
        }
    }
    else if (planDisplay == SHOWING || colorPickerScreen == SHOWING)
    {
        /* Navigate between Edit and Back button in top menu */
        switch (topMenu)
        {
        case TOP_MENU_NONE:
            /* When no element selected, when left pressed, go to Back button */
            backgroundTopMenuButton.invalidate();
            backgroundTopMenuButton.moveTo(0, 0);
            backgroundTopMenuButton.setVisible(true);
            iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_ACTIVE_ID));
            backgroundTopMenuButton.invalidate();
            topMenu = TOP_MENU_BACK;
            break;
        case TOP_MENU_BACK:
            /* When element Back selected, and left pressed, do nothing */
            break;
        case TOP_MENU_EDIT:
            /* When element Edit selected, and left pressed, go to Back button */
            backgroundTopMenuButton.invalidate();
            iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_NORMAL_ID));
            iconEditTopMenu.invalidate();
            backgroundTopMenuButton.moveTo(0, 0);
            backgroundTopMenuButton.setVisible(true);
            iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_ACTIVE_ID));
            backgroundTopMenuButton.invalidate();
            topMenu = TOP_MENU_BACK;
            break;
        }
    }
}

void MainView::selectPressed()
{
    goToIdleStateTimeOutCounter = 0;

    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && selectedMenu != MENU_NONE
            && planDisplay == NOT_SHOWING
            && colorPickerScreen == NOT_SHOWING
            && idleScreen == NOT_SHOWING
       )
    {
        arrow_up.setVisible(false);
        arrow_up.invalidate();
        iconDegree.setVisible(false);
        iconDegree.invalidate();
        containerSetTemperature.setVisible(false);
        containerSetTemperature.invalidate();
        showMenuTickCounter = 0;

        switch (selectedMenu)
        {
        case MENU_MODE :
            stateModeMenu = ANIMATING_IN;
            break;
        case MENU_PLAN :
            statePlanMenu = ANIMATING_IN;
            break;
        case MENU_TIMER :
            stateTimerMenu = ANIMATING_IN;
            break;
        case MENU_SETUP :
            stateSetupMenu = ANIMATING_IN;
        case MENU_NONE :
            break;
        }

        //set menu elements state
        menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
        menuCursorPosition = LEVEL_1;

        switch (selectedMenu)
        {
        case MENU_NONE:
            break;
        case MENU_MODE:
            textAreaMenu4.setTypedText(TypedText(T_MENU_FAN_ANGLE));
            textAreaMenu3.setTypedText(TypedText(T_MENU_SILENT));
            textAreaMenu2.setTypedText(TypedText(T_MENU_BOOST));
            textAreaMenu1.setTypedText(TypedText(T_MENU_ECO));

            //check mark according to modeState
            menuIconSelected.setY(modeState.ecoActive ? 236 : (modeState.boostActive ? 202 : 168));

            break;
        case MENU_PLAN:
            selectedPlan = PLAN_DAY;
            textAreaMenu4.setTypedText(TypedText(T_MENU_CUSTOM_PLAN));
            textAreaMenu3.setTypedText(TypedText(T_MENU_MONTHLY));
            textAreaMenu2.setTypedText(TypedText(T_MENU_WEEK));
            textAreaMenu1.setTypedText(TypedText(T_MENU_DAY));
            break;
        case MENU_TIMER:
            textAreaMenu4.setTypedText(TypedText(T_MENU_CUSTOM_TIMER));
            textAreaMenu3.setTypedText(TypedText(T_MENU_6_HOURS));
            textAreaMenu2.setTypedText(TypedText(T_MENU_4_HOURS));
            textAreaMenu1.setTypedText(TypedText(T_MENU_2_HOURS));

            //check mark according to timerState
            menuIconSelected.setY(timerState.hours2Active ? 236 : (timerState.hours4Active ? 202 : 168));
            break;
        case MENU_SETUP:
            selectedSetup = SETUP_COLOR_PICKER;
            textAreaMenu4.setTypedText(TypedText(T_MENU_CUSTOMIZE));
            textAreaMenu3.setTypedText(TypedText(T_MENU_THEMES));
            textAreaMenu2.setTypedText(TypedText(T_MENU_TIMER_SETTINGS));
            textAreaMenu1.setTypedText(TypedText(T_MENU_COLOR_PICKER));
            break;
        }

    }
    else if (stateModeMenu == SHOWING)
    {
        switch (menuCursorPosition)
        {
        case LEVEL_1: //eco
            menuIconSelected.moveTo(menuIconSelected.getX(), 236);
            modeState.ecoActive = true;
            modeState.boostActive = false;
            modeState.silentActive = false;
            break;
        case LEVEL_2: //boost
            menuIconSelected.moveTo(menuIconSelected.getX(), 202);
            modeState.boostActive = true;
            modeState.ecoActive = false;
            modeState.silentActive = false;
            break;
        case LEVEL_3: //silent
            menuIconSelected.moveTo(menuIconSelected.getX(), 168);
            modeState.silentActive = true;
            modeState.ecoActive = false;
            modeState.boostActive = false;
            break;
        case LEVEL_4: //fan angle
            break;
        case LEVEL_DOWN_BUTTON:
            removeMenu(stateModeMenu);
            break;
        }
        updateBoostIcon();
        updateEcoIcon();
        updateSilentIcon();
    }
    else if (statePlanMenu == SHOWING)
    {
        if (selectedPlan != PLAN_NONE)
        {
            planClicked = true;
            removeMenu(statePlanMenu);
        }

        if (menuCursorPosition == LEVEL_DOWN_BUTTON)
        {
            removeMenu(statePlanMenu);
        }
    }
    else if (stateTimerMenu == SHOWING)
    {
        switch (menuCursorPosition)
        {
        case LEVEL_1: //2 HOURS
            menuIconSelected.moveTo(menuIconSelected.getX(), 236);
            timerState.hours2Active = true;
            timerState.hours4Active = false;
            timerState.hours6Active = false;
            timerState.timerActive = true;
            break;
        case LEVEL_2: //4 HOURS
            menuIconSelected.moveTo(menuIconSelected.getX(), 202);
            timerState.hours2Active = false;
            timerState.hours4Active = true;
            timerState.hours6Active = false;
            timerState.timerActive = true;
            break;
        case LEVEL_3: //6 HOURS
            menuIconSelected.moveTo(menuIconSelected.getX(), 168);
            timerState.hours2Active = false;
            timerState.hours4Active = false;
            timerState.hours6Active = true;
            timerState.timerActive = true;
            break;
        case LEVEL_4: //CUSTOM TIMER
            break;
        case LEVEL_DOWN_BUTTON:
            removeMenu(stateTimerMenu);
            break;
        }
        if (timerState.timerActive == true)
        {
            menuIconSelected.setVisible(true);
            menuIconSelected.invalidate();
        }
        updateTimerIcon();
    }
    else if (stateSetupMenu == SHOWING)
    {
        if (selectedSetup == SETUP_COLOR_PICKER)
        {
            colorPickerClicked = true;
            removeMenu(stateSetupMenu);
        }
        if (menuCursorPosition == LEVEL_DOWN_BUTTON)
        {
            removeMenu(stateSetupMenu);
        }
    }
    else if (planDisplay == SHOWING)
    {
        if (topMenu == TOP_MENU_BACK)
        {
            /* Go back to main screen */
            backgroundTopMenuButton.setVisible(false);
            iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_NORMAL_ID));
            iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_NORMAL_ID));
            topMenu = TOP_MENU_NONE;
            planClicked = false;
            showMenuTickCounter = 0;
            planDisplay = ANIMATING_OUT;
        }
    }
    else if (colorPickerScreen == SHOWING)
    {
        if (topMenu == TOP_MENU_BACK)
        {
            /* Go back to main screen */
            backgroundTopMenuButton.setVisible(false);
            iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_NORMAL_ID));
            iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_NORMAL_ID));
            topMenu = TOP_MENU_NONE;
            colorPickerClicked = false;
            showMenuTickCounter = 0;
            colorPickerScreen = ANIMATING_OUT;
        }
    }
    else if (idleScreen == SHOWING)
    {
        showMenuTickCounter = 0;
        idleScreen = ANIMATING_OUT;
    }
}

void MainView::updateTimerIcon()
{
    BitmapId icon = timerState.timerActive ? BITMAP_ICON_SMALL_TIMER_ACTIVE_ID : BITMAP_ICON_SMALL_TIMER_NORMAL_ID;
    if (icon != iconTimer.getBitmap())
    {
        iconTimer.setBitmap(Bitmap(icon));
        iconTimer.invalidate();
        textTimer.setColor(timerState.timerActive ? color_dirty_white : color_orange);
        textTimer.invalidate();
    }
}

void MainView::updateBoostIcon()
{
    BitmapId icon = modeState.boostActive ? BITMAP_ICON_SMALL_BOOST_ACTIVE_ID : BITMAP_ICON_SMALL_BOOST_NORMAL_ID;
    if (icon != iconBoost.getBitmap())
    {
        iconBoost.setBitmap(Bitmap(icon));
        iconBoost.invalidate();
        textBoost.setColor(modeState.boostActive ? color_dirty_white : color_orange);
        textBoost.invalidate();
    }
}

void MainView::updateEcoIcon()
{
    BitmapId icon = modeState.ecoActive ? BITMAP_ICON_SMALL_ECO_ACTIVE_ID : BITMAP_ICON_SMALL_ECO_NORMAL_ID;
    if (icon != iconEco.getBitmap())
    {
        iconEco.setBitmap(Bitmap(icon));
        iconEco.invalidate();
        textEco.setColor(modeState.ecoActive ? color_dirty_white : color_orange);
        textEco.invalidate();
    }
}

void MainView::updateSilentIcon()
{
    BitmapId icon = modeState.silentActive ? BITMAP_ICON_SMALL_SILENT_ACTIVE_ID : BITMAP_ICON_SMALL_SILENT_NORMAL_ID;
    if (icon != iconSilent.getBitmap())
    {
        iconSilent.setBitmap(Bitmap(icon));
        iconSilent.invalidate();
        textSilent.setColor(modeState.silentActive ? color_dirty_white : color_orange);
        textSilent.invalidate();
    }
}

void MainView::updateAngleIcon()
{
    BitmapId icon = modeState.angleActive ? BITMAP_ICON_SMALL_ANGLE_ACTIVE_ID : BITMAP_ICON_SMALL_ANGLE_NORMAL_ID;
    if (icon != iconAngle.getBitmap())
    {
        iconAngle.setBitmap(Bitmap(icon));
        iconAngle.invalidate();
        textAngle.setColor(modeState.angleActive ? color_dirty_white : color_orange);
        textAngle.invalidate();
    }
}

void MainView::handleTickEvent()
{
    /* No timer if the screen is displaying a plan mode or the color picker screen.
     * The only way to go out of a screen different from the main screen is to select the back button
     */
    if (planDisplay == NOT_SHOWING
            && colorPickerScreen == NOT_SHOWING
            && idleScreen == NOT_SHOWING)
    {
        keyTimeOutCounter++;
        goToIdleStateTimeOutCounter++;
    }
    //cancel selected menu entry (bottom menu bar)
    if (keyTimeOutCounter == 480)
    {
        keyTimeOutCounter = 0;
        if (stateModeMenu == NOT_SHOWING
                && statePlanMenu == NOT_SHOWING
                && stateTimerMenu == NOT_SHOWING
                && stateSetupMenu == NOT_SHOWING
                && selectedMenu != MENU_NONE)
        {
            goToIdleStateTimeOutCounter = 0;
            boxModeSelected.setVisible(false);
            switch (selectedMenu)
            {
            case MENU_NONE:
                break;
            case MENU_MODE:
                menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_NORMAL_ID));
                textAreaMode.setColor(color_golden_yellow);
                break;
            case MENU_PLAN:
                menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
                textAreaPlan.setColor(color_golden_yellow);
                break;
            case MENU_TIMER:
                menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_NORMAL_ID));
                textAreaTimer.setColor(color_golden_yellow);
                break;
            case MENU_SETUP:
                menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
                textAreaSetup.setColor(color_golden_yellow);
                break;
            }
            boxModeSelected.invalidate(); // Invalidates the whole area of a menu item
            selectedMenu = MENU_NONE;
        }
        if (stateModeMenu == SHOWING)
        {
            removeMenu(stateModeMenu);
        }
        else if (statePlanMenu == SHOWING)
        {
            removeMenu(statePlanMenu);
        }
        else if (stateTimerMenu == SHOWING)
        {
            removeMenu(stateTimerMenu);
        }
        else if (stateSetupMenu == SHOWING)
        {
            removeMenu(stateSetupMenu);
        }
    }

    if (goToIdleStateTimeOutCounter == 600) //After 10 sec
    {
        goToIdleStateTimeOutCounter = 0;
        if (stateModeMenu == NOT_SHOWING
                && statePlanMenu == NOT_SHOWING
                && stateTimerMenu == NOT_SHOWING
                && stateSetupMenu == NOT_SHOWING
                && selectedMenu == MENU_NONE)
        {
            showMenuTickCounter = 0;
            idleScreen = ANIMATING_IN;
        }
    }

    if (idleScreen == SHOWING)
    {
        //int16_t innerCircleArcStart = innerCircle.getArcStart();
        if (innerCircle.getArcEnd() == 360)
        {
            innerCircle.updateArc(innerCircle.getArcStart() - 359, innerCircle.getArcEnd() - 359);
        }
        else
        {
            innerCircle.updateArc(innerCircle.getArcStart() + 1, innerCircle.getArcEnd() + 1);
        }

        if (outerCircle.getArcEnd() == -360)
        {
            outerCircle.updateArc(outerCircle.getArcStart() + 359, outerCircle.getArcEnd() + 359);
        }
        else
        {
            outerCircle.updateArc(outerCircle.getArcStart() - 1, outerCircle.getArcEnd() - 1);
        }
    }

    /************************************************************************************************************/
    /*********************************************ANIMATING OUT**************************************************/
    /************************************************************************************************************/

    if (stateModeMenu == ANIMATING_OUT)
    {
        showMenuTickCounter++;
        switch (showMenuTickCounter)
        {
        case 1:
            menuIconSelected.setVisible(false);
            arrow_up.setVisible(true);
            arrow_up.invalidate();
            iconDegree.setVisible(true);
            iconDegree.invalidate();
            containerSetTemperature.setVisible(true);
            containerSetTemperature.invalidate();
            containerMenuDown.setVisible(false);
            containerMenuDown.invalidate();
            break;
        case 3:
            arrowMenu4.setVisible(false);
            containerMenu4.setVisible(false);
            containerMenu4.invalidate();
            break;
        case 5:
            containerMenu3.setVisible(false);
            containerMenu3.invalidate();
            break;
        case 7:
            containerMenu2.setVisible(false);
            containerMenu2.invalidate();
            break;
        case 9:
            containerMenu1.setVisible(false);
            containerMenu1.invalidate();
            stateModeMenu = NOT_SHOWING;
            keyTimeOutCounter = 0;
            break;
        }
    }

    if (statePlanMenu == ANIMATING_OUT)
    {
        showMenuTickCounter++;
        switch (showMenuTickCounter)
        {
        case 1:
            if (!planClicked)
            {
                arrow_up.setVisible(true);
                arrow_up.invalidate();
                iconDegree.setVisible(true);
                iconDegree.invalidate();
                containerSetTemperature.setVisible(true);
                containerSetTemperature.invalidate();
            }
            containerMenuDown.setVisible(false);
            containerMenuDown.invalidate();
            break;
        case 3:
            arrowMenu4.setVisible(false);
            containerMenu4.setVisible(false);
            containerMenu4.invalidate();
            break;
        case 5:
            arrowMenu3.setVisible(false);
            containerMenu3.setVisible(false);
            containerMenu3.invalidate();
            break;
        case 7:
            arrowMenu2.setVisible(false);
            containerMenu2.setVisible(false);
            containerMenu2.invalidate();
            break;
        case 9:
            arrowMenu1.setVisible(false);
            containerMenu1.setVisible(false);
            containerMenu1.invalidate();

            statePlanMenu = NOT_SHOWING;
            keyTimeOutCounter = 0;
            if (planClicked)
            {
                planDisplay = ANIMATING_IN;
                showMenuTickCounter = 0;
            }
            break;
        }
    }

    if (stateTimerMenu == ANIMATING_OUT)
    {
        showMenuTickCounter++;
        switch (showMenuTickCounter)
        {
        case 1:
            menuIconSelected.setVisible(false);
            arrow_up.setVisible(true);
            arrow_up.invalidate();
            iconDegree.setVisible(true);
            iconDegree.invalidate();
            containerSetTemperature.setVisible(true);
            containerSetTemperature.invalidate();
            containerMenuDown.setVisible(false);
            containerMenuDown.invalidate();
            break;
        case 3:
            arrowMenu4.setVisible(false);
            containerMenu4.setVisible(false);
            containerMenu4.invalidate();
            break;
        case 5:
            containerMenu3.setVisible(false);
            containerMenu3.invalidate();
            break;
        case 7:
            containerMenu2.setVisible(false);
            containerMenu2.invalidate();
            break;
        case 9:
            containerMenu1.setVisible(false);
            containerMenu1.invalidate();
            stateTimerMenu = NOT_SHOWING;
            keyTimeOutCounter = 0;
            break;
        }
    }

    if (stateSetupMenu == ANIMATING_OUT)
    {
        showMenuTickCounter++;
        switch (showMenuTickCounter)
        {
        case 1:
            // menuIconSelected.setVisible(false);
            if (!colorPickerClicked)
            {
                arrow_up.setVisible(true);
                arrow_up.invalidate();
                iconDegree.setVisible(true);
                iconDegree.invalidate();
                containerSetTemperature.setVisible(true);
                containerSetTemperature.invalidate();
            }
            containerMenuDown.setVisible(false);
            containerMenuDown.invalidate();
            break;
        case 3:
            arrowMenu4.setVisible(false);
            containerMenu4.setVisible(false);
            containerMenu4.invalidate();
            break;
        case 5:
            arrowMenu3.setVisible(false);
            containerMenu3.setVisible(false);
            containerMenu3.invalidate();
            break;
        case 7:
            arrowMenu2.setVisible(false);
            containerMenu2.setVisible(false);
            containerMenu2.invalidate();
            break;
        case 9:
            arrowMenu1.setVisible(false);
            containerMenu1.setVisible(false);
            containerMenu1.invalidate();

            stateSetupMenu = NOT_SHOWING;
            keyTimeOutCounter = 0;
            if (colorPickerClicked)
            {
                colorPickerScreen = ANIMATING_IN;
                showMenuTickCounter = 0;
            }
            break;
        }
    }

    if (planDisplay == ANIMATING_OUT)
    {
        showMenuTickCounter++;
        if (showMenuTickCounter == 2)
        {
            imagePlan.setVisible(false);
            imagePlan.invalidate();
        }
        if (showMenuTickCounter == 5)
        {
            containerTopMenu.setVisible(false);
            containerTopMenu.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            arrow_up.setVisible(true);
            arrow_up.invalidate();
            iconDegree.setVisible(true);
            iconDegree.invalidate();
            containerSetTemperature.setVisible(true);
            containerSetTemperature.invalidate();
            planDisplay = NOT_SHOWING;
        }
    }

    if (colorPickerScreen == ANIMATING_OUT)
    {
        showMenuTickCounter++;
        if (showMenuTickCounter == 2)
        {
            containerColorPicker.setVisible(false);
            containerColorPicker.invalidate();
        }
        if (showMenuTickCounter == 5)
        {
            ornament_left.invalidate();
            ornament_left.moveTo(ornament_left.getX(), 191);
            ornament_left.invalidate();
            ornament_right.invalidate();
            ornament_right.moveTo(ornament_right.getX(), 191);
            ornament_right.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            containerTopMenu.setVisible(false);
            containerTopMenu.invalidate();
        }
        if (showMenuTickCounter == 9)
        {
            containerMainTopMenu.setVisible(true);
            containerMainTopMenu.invalidate();
            arrow_down.setVisible(true);
            arrow_down.invalidate();
            containerBottomMenu.setVisible(true);
            containerBottomMenu.invalidate();
        }
        if (showMenuTickCounter == 11)
        {
            arrow_up.setVisible(true);
            arrow_up.invalidate();
            iconDegree.setVisible(true);
            iconDegree.invalidate();
            containerSetTemperature.setVisible(true);
            containerSetTemperature.invalidate();
            colorPickerScreen = NOT_SHOWING;
        }
    }
    if (idleScreen == ANIMATING_OUT)
    {
        // Come back to main screen
        showMenuTickCounter++;
        if (showMenuTickCounter == 2)
        {
            containerIdleCircles.setVisible(false);
            containerIdleCircles.invalidate();
        }
        if (showMenuTickCounter == 5)
        {
            ornament_left.invalidate();
            ornament_left.moveTo(ornament_left.getX(), 191);
            ornament_left.invalidate();
            ornament_right.invalidate();
            ornament_right.moveTo(ornament_right.getX(), 191);
            ornament_right.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            containerBottomMenu.setVisible(true);
            containerBottomMenu.invalidate();
        }
        if (showMenuTickCounter == 9)
        {
            textTimer.setVisible(true);
            textTimer.invalidate();
            textBoost.setVisible(true);
            textBoost.invalidate();
            textEco.setVisible(true);
            textEco.invalidate();
            textSilent.setVisible(true);
            textSilent.invalidate();
            textAngle.setVisible(true);
            textAngle.invalidate();
            arrow_up.setVisible(true);
            arrow_up.invalidate();
            arrow_down.setVisible(true);
            arrow_down.invalidate();
            iconDegree.setVisible(true);
            iconDegree.invalidate();
            innerCircle.updateArc(130, 310);
            outerCircle.updateArc(-20, 160);
            idleScreen = NOT_SHOWING;

            containerSetTemperature.invalidate();
            containerSetTemperature.setY(containerSetTemperature.getY() - 14);
            containerSetTemperature.invalidate();
        }
    }


    /***********************************************************************************************************/
    /*********************************************ANIMATING IN**************************************************/
    /***********************************************************************************************************/

    if (stateModeMenu == ANIMATING_IN)
    {
        showMenuTickCounter++;
        if (showMenuTickCounter == 2)
        {
            if (setupMenuModeClick == true)
            {
                menuCursorPosition = LEVEL_DOWN_BUTTON;
                menuIconSelected.setVisible(true);
                textAreaMenu1.setColor(color_dark_red);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ID));
                containerMenu1.setVisible(true);
                containerMenu1.invalidate();
                if (modeState.silentActive == false && modeState.ecoActive == false && modeState.boostActive == false)
                {
                    menuIconSelected.setVisible(false);
                    menuCursorPosition = LEVEL_DOWN_BUTTON;
                }
                else if (modeState.ecoActive == true)
                {
                    iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ACTIVE_ID));
                    menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                    textAreaMenu1.setColor(color_dirty_white);
                    menuCursorPosition = LEVEL_1;
                }
                else if (modeState.silentActive == true)
                {
                    iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ACTIVE_ID));
                    menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                    textAreaMenu3.setColor(color_dirty_white);
                    menuCursorPosition = LEVEL_3;
                }
                else if (modeState.boostActive == true)
                {
                    iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ACTIVE_ID));
                    menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                    textAreaMenu2.setColor(color_dirty_white);
                    menuCursorPosition = LEVEL_2;
                }
            }
            else
            {
                menuIconSelected.setVisible(true);
                textAreaMenu1.setColor(color_dirty_white);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ACTIVE_ID));
                containerMenu1.setVisible(true);
                containerMenu1.invalidate();
            }
        }
        if (showMenuTickCounter == 5)
        {
            if (modeState.boostActive == true && setupMenuModeClick == true)
            {
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ACTIVE_ID));
            }
            else
            {
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ID));
            }
            containerMenu2.setVisible(true);
            containerMenu2.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            if (modeState.silentActive == true && setupMenuModeClick == true)
            {
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ACTIVE_ID));
            }
            else
            {
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ID));
            }
            containerMenu3.setVisible(true);
            containerMenu3.invalidate();
        }
        if (showMenuTickCounter == 9)
        {
            arrowMenu4.setVisible(true);
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
            containerMenu4.setVisible(true);
            containerMenu4.invalidate();
        }
        if (showMenuTickCounter == 11)
        {
            containerMenuDown.setVisible(true);
            containerMenuDown.invalidate();
            stateModeMenu = SHOWING;
            keyTimeOutCounter = 0;
            setupMenuModeClick = false;
        }
    }
    else if (statePlanMenu == ANIMATING_IN)
    {
        showMenuTickCounter++;
        if (showMenuTickCounter == 2)
        {
            if (setupMenuPlanClick == true)
            {
                menuCursorPosition = LEVEL_DOWN_BUTTON;
                textAreaMenu1.setColor(color_dark_red);
                arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                arrowMenu1.setVisible(true);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
                containerMenu1.setVisible(true);
                containerMenu1.invalidate();
                setupMenuPlanClick = false;
            }
            else
            {
                textAreaMenu1.setColor(color_dirty_white);
                arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                arrowMenu1.setVisible(true);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
                containerMenu1.setVisible(true);
                containerMenu1.invalidate();
            }
        }
        if (showMenuTickCounter == 5)
        {
            arrowMenu2.setVisible(true);
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
            containerMenu2.setVisible(true);
            containerMenu2.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            arrowMenu3.setVisible(true);
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
            containerMenu3.setVisible(true);
            containerMenu3.invalidate();
        }
        if (showMenuTickCounter == 9)
        {
            arrowMenu4.setVisible(true);
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            containerMenu4.setVisible(true);
            containerMenu4.invalidate();
        }
        if (showMenuTickCounter == 11)
        {
            containerMenuDown.setVisible(true);
            containerMenuDown.invalidate();
            statePlanMenu = SHOWING;
            keyTimeOutCounter = 0;
        }
    }
    else if (stateTimerMenu == ANIMATING_IN)
    {
        showMenuTickCounter++;
        if (showMenuTickCounter == 2)
        {
            if (timerState.timerActive == true)
            {
                menuIconSelected.setVisible(true);
            }
            if (setupMenuTimerClick == true)
            {
                menuCursorPosition = LEVEL_DOWN_BUTTON;
                textAreaMenu1.setColor(color_dark_red);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
                containerMenu1.setVisible(true);
                containerMenu1.invalidate();
                if (timerState.hours2Active == false && timerState.hours4Active == false && timerState.hours6Active == false)
                {
                    menuIconSelected.setVisible(false);
                    menuCursorPosition = LEVEL_DOWN_BUTTON;
                }
                else if (timerState.hours2Active == true)
                {
                    iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
                    menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                    textAreaMenu1.setColor(color_dirty_white);
                    menuCursorPosition = LEVEL_1;
                }
                else if (timerState.hours6Active == true)
                {
                    iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
                    menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                    textAreaMenu3.setColor(color_dirty_white);
                    menuCursorPosition = LEVEL_3;
                }
                else if (timerState.hours4Active == true)
                {
                    iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
                    menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
                    textAreaMenu2.setColor(color_dirty_white);
                    menuCursorPosition = LEVEL_2;
                }
            }
            else
            {
                textAreaMenu1.setColor(color_dirty_white);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
                containerMenu1.setVisible(true);
                containerMenu1.invalidate();
            }
        }
        if (showMenuTickCounter == 5)
        {
            if (timerState.hours4Active == true && setupMenuTimerClick == true)
            {
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
            }
            else
            {
                iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            }
            containerMenu2.setVisible(true);
            containerMenu2.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            if (timerState.hours6Active == true && setupMenuTimerClick == true)
            {
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
            }
            else
            {
                iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            }
            containerMenu3.setVisible(true);
            containerMenu3.invalidate();
        }
        if (showMenuTickCounter == 9)
        {
            arrowMenu4.setVisible(true);
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            containerMenu4.setVisible(true);
            containerMenu4.invalidate();
        }
        if (showMenuTickCounter == 11)
        {
            containerMenuDown.setVisible(true);
            containerMenuDown.invalidate();
            stateTimerMenu = SHOWING;
            keyTimeOutCounter = 0;
            setupMenuTimerClick = false ;
        }
    }
    else if (stateSetupMenu == ANIMATING_IN)
    {
        showMenuTickCounter++;
        if (showMenuTickCounter == 2)
        {
            if (setupMenuSetupClick == true)
            {
                menuCursorPosition = LEVEL_DOWN_BUTTON;
                textAreaMenu1.setColor(color_dark_red);
                arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
                arrowMenu1.setVisible(true);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_COLOR_PICKER_ID));
                containerMenu1.setVisible(true);
                containerMenu1.invalidate();
                setupMenuSetupClick = false;
            }
            else
            {
                textAreaMenu1.setColor(color_dirty_white);
                arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
                arrowMenu1.setVisible(true);
                iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_COLOR_PICKER_ACTIVE_ID));
                containerMenu1.setVisible(true);
                containerMenu1.invalidate();
            }
        }
        if (showMenuTickCounter == 5)
        {
            arrowMenu2.setVisible(true);
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ID));
            arrowMenu2.setAlpha(160);
            iconMenu2.setAlpha(180);
            textAreaMenu2.setAlpha(140);
            containerMenu2.setVisible(true);
            containerMenu2.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            arrowMenu3.setVisible(true);
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ID));
            arrowMenu3.setAlpha(160);
            iconMenu3.setAlpha(180);
            textAreaMenu3.setAlpha(140);
            containerMenu3.setVisible(true);
            containerMenu3.invalidate();
        }
        if (showMenuTickCounter == 9)
        {
            arrowMenu4.setVisible(true);
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            containerMenu4.setVisible(true);
            containerMenu4.invalidate();
        }
        if (showMenuTickCounter == 11)
        {
            containerMenuDown.setVisible(true);
            containerMenuDown.invalidate();
            stateSetupMenu = SHOWING;
            keyTimeOutCounter = 0;
        }
    }

    else if (planDisplay == ANIMATING_IN)
    {
        showMenuTickCounter++;
        if (showMenuTickCounter == 1)
        {
            switch (selectedPlan)
            {
            case PLAN_NONE:
                break;
            case PLAN_DAY:
                imagePlan.setBitmap(touchgfx::Bitmap(BITMAP_DAY_PLAN_ID));
                textTopMenu.setTypedText(touchgfx::TypedText(T_TOP_MENU_DAY));
                break;
            case PLAN_WEEK:
                imagePlan.setBitmap(touchgfx::Bitmap(BITMAP_WEEK_PLAN_ID));
                textTopMenu.setTypedText(touchgfx::TypedText(T_TOP_MENU_WEEK));
                break;
            case PLAN_MONTH:
                imagePlan.setBitmap(touchgfx::Bitmap(BITMAP_MONTH_PLAN_ID));
                textTopMenu.setTypedText(touchgfx::TypedText(T_TOP_MENU_MONTH));
                break;
            case PLAN_CUSTOM:
                break;
            }
        }
        if (showMenuTickCounter == 2)
        {
            containerTopMenu.setVisible(true);
            containerTopMenu.invalidate();
        }
        if (showMenuTickCounter == 5)
        {
            imagePlan.setVisible(true);
            imagePlan.invalidate();
            planDisplay = SHOWING;
        }
    }

    else if (colorPickerScreen == ANIMATING_IN)
    {
        showMenuTickCounter++;
        if (showMenuTickCounter == 1)
        {
            textTopMenu.setTypedText(touchgfx::TypedText(T_TOP_MENU_COLOR_PICKER));
        }
        if (showMenuTickCounter == 2)
        {
            containerMainTopMenu.setVisible(false);
            containerMainTopMenu.invalidate();
            arrow_down.setVisible(false);
            arrow_down.invalidate();
            containerBottomMenu.setVisible(false);
            containerBottomMenu.invalidate();
        }
        if (showMenuTickCounter == 5)
        {
            containerTopMenu.setVisible(true);
            containerTopMenu.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            ornament_left.invalidate();
            ornament_left.moveTo(ornament_left.getX(), 248);
            ornament_left.invalidate();
            ornament_right.invalidate();
            ornament_right.moveTo(ornament_right.getX(), 248);
            ornament_right.invalidate();
        }
        if (showMenuTickCounter == 9)
        {
            containerColorPicker.setVisible(true);
            containerColorPicker.invalidate();
            colorPickerScreen = SHOWING;
        }
    }

    else if (idleScreen == ANIMATING_IN)
    {
        // Go to IDLE screen
        showMenuTickCounter++;
        if (showMenuTickCounter == 2)
        {
            textTimer.setVisible(false);
            textTimer.invalidate();
            textBoost.setVisible(false);
            textBoost.invalidate();
            textEco.setVisible(false);
            textEco.invalidate();
            textSilent.setVisible(false);
            textSilent.invalidate();
            textAngle.setVisible(false);
            textAngle.invalidate();
            arrow_up.setVisible(false);
            arrow_up.invalidate();
            arrow_down.setVisible(false);
            arrow_down.invalidate();
            iconDegree.setVisible(false);
            iconDegree.invalidate();
        }
        if (showMenuTickCounter == 5)
        {
            containerBottomMenu.setVisible(false);
            containerBottomMenu.invalidate();
        }
        if (showMenuTickCounter == 7)
        {
            ornament_left.invalidate();
            ornament_left.moveTo(ornament_left.getX(), 248);
            ornament_left.invalidate();
            ornament_right.invalidate();
            ornament_right.moveTo(ornament_right.getX(), 248);
            ornament_right.invalidate();
        }
        if (showMenuTickCounter == 9)
        {
            containerSetTemperature.invalidate();
            containerSetTemperature.setY(containerSetTemperature.getY() + 14);
            containerSetTemperature.invalidate();

            containerIdleCircles.setVisible(true);
            containerIdleCircles.invalidate();
            idleScreen = SHOWING;
        }
    }
}

void MainView::removeMenu(ShowingModeMenu& menu)
{
    menuDownIcon.setBitmap(Bitmap(BITMAP_ARROW_TAB_NORMAL_ID));
    textAreaMenu2.setColor(color_dark_red);
    textAreaMenu3.setColor(color_dark_red);
    textAreaMenu4.setColor(color_dark_red);
    arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
    arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
    arrowMenu2.setAlpha(255);
    arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
    arrowMenu3.setAlpha(255);
    arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
    textAreaMenu2.setAlpha(255);
    textAreaMenu3.setAlpha(255);
    iconMenu2.setAlpha(255);
    iconMenu3.setAlpha(255);


    showMenuTickCounter = 0;
    menu = ANIMATING_OUT;
}

void MainView::showMenuMode()
{
    //THE FONCTION THIS CALL WHEN THE menuIconModeFlexButton is CLICKED
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    // time to 0
    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && planDisplay == NOT_SHOWING
            && colorPickerScreen == NOT_SHOWING
            && idleScreen == NOT_SHOWING
       ) // if nothing this display
    {
        setupMenuModeClick = true; // the menuIconModeFlexButton is click

        // update of the select menu
        menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_MENU_SELECTED_ID));
        menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
        menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
        menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_NORMAL_ID));
        textAreaMode.setColor(color_dark_red);
        textAreaPlan.setColor(color_golden_yellow);
        textAreaSetup.setColor(color_golden_yellow);
        textAreaTimer.setColor(color_golden_yellow);
        boxModeSelected.moveTo(0, 0);
        boxModeSelected.setVisible(true);
        boxModeSelected.invalidate();

        selectedMenu = MENU_MODE;// select the menu display here MODE

        //  no displays items that are not required
        arrow_up.setVisible(false);
        arrow_up.invalidate();
        iconDegree.setVisible(false);
        iconDegree.invalidate();
        containerSetTemperature.setVisible(false);
        containerSetTemperature.invalidate();

        showMenuTickCounter = 0; // time to 0

        stateModeMenu = ANIMATING_IN;// put the animating in of the ModeMenu

        // put the bitmap as normal for the submenu
        menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));

        //put the name of the submenu
        textAreaMenu4.setTypedText(TypedText(T_MENU_FAN_ANGLE));
        textAreaMenu3.setTypedText(TypedText(T_MENU_SILENT));
        textAreaMenu2.setTypedText(TypedText(T_MENU_BOOST));
        textAreaMenu1.setTypedText(TypedText(T_MENU_ECO));

        // update the menuIconSelected
        menuIconSelected.setVisible(true);
        menuIconSelected.setY(modeState.ecoActive ? 236 : (modeState.boostActive ? 202 : 168));

        // remove the arrowMenu of the other menus
        arrowMenu2.setVisible(false);
        arrowMenu3.setVisible(false);
        arrowMenu1.setVisible(false);

    }
    else if (stateModeMenu == SHOWING)
    {
        //if the menu MODE is showing, the menu MODE is remove
        // update of the select menu
        menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_NORMAL_ID));
        textAreaMode.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();

        removeMenu(stateModeMenu); // remove the menu Mode
        selectedMenu = MENU_NONE;
    }
    else if (statePlanMenu == SHOWING)
    {
        //if the menu PLAN is showing, close the menu
        buttonDownClicked();
    }
    else if (stateTimerMenu == SHOWING)
    {
        //if the menu TIMER is showing, close the menu
        buttonDownClicked();
    }
    else if (stateSetupMenu == SHOWING)
    {
        //if the menu SETUP is showing, close the menu
        buttonDownClicked();
    }
}

void MainView::showMenuPlan()
{
    //THE FONCTION THIS CALL WHEN THE menuIconPlanFlexButton is CLICKED
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    menuCursorPosition = LEVEL_DOWN_BUTTON;
    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && planDisplay == NOT_SHOWING
            && colorPickerScreen == NOT_SHOWING
            && idleScreen == NOT_SHOWING)
    {
        // if nothing this display
        setupMenuPlanClick = true;
        menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_MENU_SELECTED_ID));
        menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_NORMAL_ID));
        menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
        menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_NORMAL_ID));
        textAreaMode.setColor(color_golden_yellow);
        textAreaPlan.setColor(color_dark_red);
        textAreaSetup.setColor(color_golden_yellow);
        textAreaTimer.setColor(color_golden_yellow);
        boxModeSelected.setVisible(true);
        boxModeSelected.moveTo(60, 0);
        boxModeSelected.invalidate();
        selectedMenu = MENU_PLAN;
        arrow_up.setVisible(false);
        arrow_up.invalidate();
        iconDegree.setVisible(false);
        iconDegree.invalidate();
        containerSetTemperature.setVisible(false);
        containerSetTemperature.invalidate();
        showMenuTickCounter = 0;
        statePlanMenu = ANIMATING_IN;
        menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        textAreaMenu4.setTypedText(TypedText(T_MENU_CUSTOM_PLAN));
        textAreaMenu3.setTypedText(TypedText(T_MENU_MONTHLY));
        textAreaMenu2.setTypedText(TypedText(T_MENU_WEEK));
        textAreaMenu1.setTypedText(TypedText(T_MENU_DAY));
        arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        menuIconSelected.setVisible(false);
    }
    else if (statePlanMenu == SHOWING)
    {
        removeMenu(statePlanMenu);
        menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
        textAreaPlan.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
        selectedMenu = MENU_NONE;
    }
    else  if (stateModeMenu == SHOWING)
    {
        buttonDownClicked();
    }
    else if (stateTimerMenu == SHOWING)
    {
        buttonDownClicked();
    }
    else if (stateSetupMenu == SHOWING)
    {
        buttonDownClicked();
    }
    else if (planDisplay == SHOWING)
    {
        //if planDisplay is showing and you click in the menuIconPlanFlexButton you call the planDisplay
        topBackButtonClicked();
    }
}

void MainView::showMenuTimer()
{
    //THE FONCTION THIS CALL WHEN THE menuIconTimerFlexButton is CLICKED
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && planDisplay == NOT_SHOWING
            && colorPickerScreen == NOT_SHOWING
            && idleScreen == NOT_SHOWING)
    {
        setupMenuTimerClick = true;
        menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_MENU_SELECTED_ID));
        menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_NORMAL_ID));
        menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
        menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
        textAreaMode.setColor(color_golden_yellow);
        textAreaPlan.setColor(color_golden_yellow);
        textAreaSetup.setColor(color_golden_yellow);
        textAreaTimer.setColor(color_dark_red);
        boxModeSelected.setVisible(true);
        boxModeSelected.moveTo(120, 0);
        boxModeSelected.invalidate();
        selectedMenu = MENU_TIMER;
        arrow_up.setVisible(false);
        arrow_up.invalidate();
        iconDegree.setVisible(false);
        iconDegree.invalidate();
        containerSetTemperature.setVisible(false);
        containerSetTemperature.invalidate();
        showMenuTickCounter = 0;
        stateTimerMenu = ANIMATING_IN;
        menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        textAreaMenu4.setTypedText(TypedText(T_MENU_CUSTOM_TIMER));
        textAreaMenu3.setTypedText(TypedText(T_MENU_6_HOURS));
        textAreaMenu2.setTypedText(TypedText(T_MENU_4_HOURS));
        textAreaMenu1.setTypedText(TypedText(T_MENU_2_HOURS));
        menuIconSelected.setVisible(true);
        menuIconSelected.setY(timerState.hours2Active ? 236 : (timerState.hours4Active ? 202 : 168));
        arrowMenu2.setVisible(false);
        arrowMenu3.setVisible(false);
        arrowMenu1.setVisible(false);
    }
    else if (stateTimerMenu == SHOWING)
    {
        menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_NORMAL_ID));
        textAreaTimer.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
        removeMenu(stateTimerMenu);
        selectedMenu = MENU_NONE;
    }
    else  if (stateModeMenu == SHOWING)
    {
        buttonDownClicked();
    }
    else if (statePlanMenu == SHOWING)
    {
        buttonDownClicked();
    }
    else if (stateSetupMenu == SHOWING)
    {
        buttonDownClicked();
    }
}

void MainView::showMenuSetup()
{
    //THE FONCTION THIS CALL WHEN THE menuIconSetupFlexButton is CLICKED
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    if (stateModeMenu == NOT_SHOWING
            && statePlanMenu == NOT_SHOWING
            && stateTimerMenu == NOT_SHOWING
            && stateSetupMenu == NOT_SHOWING
            && planDisplay == NOT_SHOWING
            && colorPickerScreen == NOT_SHOWING
            && idleScreen == NOT_SHOWING)
    {
        setupMenuSetupClick = true;
        menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_MENU_SELECTED_ID));
        menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_NORMAL_ID));
        menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
        menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_NORMAL_ID));
        textAreaMode.setColor(color_golden_yellow);
        textAreaPlan.setColor(color_golden_yellow);
        textAreaSetup.setColor(color_dark_red);
        textAreaTimer.setColor(color_golden_yellow);
        boxModeSelected.setVisible(true);
        boxModeSelected.moveTo(180, 0);
        boxModeSelected.invalidate();
        selectedMenu = MENU_SETUP;
        arrow_up.setVisible(false);
        arrow_up.invalidate();
        iconDegree.setVisible(false);
        iconDegree.invalidate();
        containerSetTemperature.setVisible(false);
        containerSetTemperature.invalidate();
        showMenuTickCounter = 0;
        stateSetupMenu = ANIMATING_IN;
        menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
        textAreaMenu4.setTypedText(TypedText(T_MENU_CUSTOMIZE));
        textAreaMenu3.setTypedText(TypedText(T_MENU_THEMES));
        textAreaMenu2.setTypedText(TypedText(T_MENU_TIMER_SETTINGS));
        textAreaMenu1.setTypedText(TypedText(T_MENU_COLOR_PICKER));
        menuIconSelected.setVisible(false);
    }
    else if (stateSetupMenu == SHOWING)
    {
        menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
        textAreaSetup.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
        removeMenu(stateSetupMenu);
        selectedMenu = MENU_NONE;
    }
    else if (statePlanMenu == SHOWING)
    {
        buttonDownClicked();
    }
    else if (stateTimerMenu == SHOWING)
    {
        buttonDownClicked();
    }
    else  if (stateModeMenu == SHOWING)
    {
        buttonDownClicked();
    }
}

void MainView::setUpTemperature()
{
    //THE FONCTION THIS CALL WHEN THE arrowUpFlexButton is CLICKED
    //it increases the temperature
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    // time to 0
    if (setTemperature < 40)
    {
        // max temerature

        setTemperature++; //increase the temperature

        //update the textArea
        Unicode::itoa(setTemperature, textAreaSetTemperatureBuffer, TEXTAREASETTEMPERATURE_SIZE, 10);
        Unicode::itoa(setTemperature, textAreaShadowSetTemperatureBuffer, TEXTAREASHADOWSETTEMPERATURE_SIZE, 10);
        containerSetTemperature.invalidate();
    }
}

void MainView::setDownTemperature()
{
    //THE FONCTION THIS CALL WHEN THE arrowDownFlexButton is CLICKED
    //it decrease  the temperature
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    // time to 0
    if (setTemperature > 0)
    {
        // min temperature

        setTemperature--; // decrease the temperature

        //update the textArea
        Unicode::itoa(setTemperature, textAreaSetTemperatureBuffer, TEXTAREASETTEMPERATURE_SIZE, 10);
        Unicode::itoa(setTemperature, textAreaShadowSetTemperatureBuffer, TEXTAREASHADOWSETTEMPERATURE_SIZE, 10);
        containerSetTemperature.invalidate();
    }
}

void MainView::menu1Clicked()
{
    //THE FONCTION THIS CALL WHEN THE menu1FlexButton is CLICKED
    // updates when you click on the frist supmenu

    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    // time to 0

    // setup of the menu 1 active
    menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
    textAreaMenu1.setColor(color_dirty_white);
    // setup the other menus to normal
    menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu2.setColor(color_dark_red);
    menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu3.setColor(color_dark_red);
    menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu4.setColor(color_dark_red);

    if (stateModeMenu == SHOWING)
    {
        // if the user is on the menu MODE
        if (modeState.ecoActive == true)
        {
            // if the eco mode is select remove it

            // update the setup of the menu eco to normal
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu1.setColor(color_dark_red);
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
            menuBackground1.invalidate();

            menuIconSelected.setVisible(false);// remove the menuIconSelected

            // the eco mode to not active
            modeState.ecoActive = false;
            updateEcoIcon();

        }
        else
        {
            // if the eco is not active, active it

            menuCursorPosition = LEVEL_1; // put the menuCursorPosition on the menu 1

            // update the image of the supmenu
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ACTIVE_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));

            //put the icon selected
            menuIconSelected.setVisible(true);
            menuIconSelected.moveTo(menuIconSelected.getX(), 236);

            // put eco mode to active
            // one mode at a time can be activated
            modeState.ecoActive = true;
            modeState.boostActive = false;
            modeState.silentActive = false;
            updateBoostIcon();
            updateEcoIcon();
            updateSilentIcon();
        }
    }
    else if (statePlanMenu == SHOWING)
    {
        // if the user is on the menu PLAN
        showMenuTickCounter = 0;
        menuCursorPosition = LEVEL_1;// put the menuCursorPosition on the menu 1

        // open the PLAN DAY
        selectedPlan = PLAN_DAY;
        backgroundTopMenuButton.setVisible(false);
        iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_NORMAL_ID));
        iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_NORMAL_ID));
        iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
        iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
        iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
        iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));

        planClicked = true;
        removeMenu(statePlanMenu);
        arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
    }
    else if (stateTimerMenu == SHOWING)
    {
        // if the user is on the menu PLAN

        if (timerState.hours2Active == true)
        {
            // if the timer 2h is select remove it

            // update the setup of the menu timer 2h to normal
            menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu1.setColor(color_dark_red);
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            menuBackground1.invalidate();

            menuIconSelected.setVisible(false);  // remove the menuIconSelected

            //remove timer
            timerState.hours2Active = false;
            timerState.timerActive = false;
            updateTimerIcon();
        }
        else
        {
            menuCursorPosition = LEVEL_1;// put the menuCursorPosition on the menu 1

            // update the image of the supmenu
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));

            //put the icon selected
            menuIconSelected.setVisible(true);
            menuIconSelected.moveTo(menuIconSelected.getX(), 236);

            // put timer 2h mode to active
            // one mode at a time can be activated
            timerState.hours2Active = true;
            timerState.hours4Active = false;
            timerState.hours6Active = false;
            timerState.timerActive = true;
            menuIconSelected.setVisible(true);
            menuIconSelected.invalidate();
            updateTimerIcon();
        }
    }
    else if (stateSetupMenu == SHOWING)
    {
        // if the user is on the menu SETUP
        showMenuTickCounter = 0;

        menuCursorPosition = LEVEL_1;// put the menuCursorPosition on the menu 1

        // open the Color Picker
        selectedSetup = SETUP_COLOR_PICKER;
        backgroundTopMenuButton.setVisible(false);
        iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_NORMAL_ID));
        iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_NORMAL_ID));
        iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_COLOR_PICKER_ACTIVE_ID));
        iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ID));
        iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ID));
        iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
        colorPickerClicked = true;
        removeMenu(stateSetupMenu);
        arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
    }
    menuBackground1.invalidate();
    menuBackground2.invalidate();
    menuBackground3.invalidate();
    menuBackground4.invalidate();
}

void MainView::menu2Clicked()
{
    //THE FONCTION THIS CALL WHEN THE menu2FlexButton is CLICKED
    // updates when you click on the second supmenu
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
    textAreaMenu2.setColor(color_dirty_white);
    menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu1.setColor(color_dark_red);
    menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu3.setColor(color_dark_red);
    menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu4.setColor(color_dark_red);

    if (stateModeMenu == SHOWING)
    {
        if (modeState.boostActive == true)
        {
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu2.setColor(color_dark_red);
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
            menuIconSelected.setVisible(false);
            modeState.boostActive = false;
            updateBoostIcon();
        }
        else
        {
            menuCursorPosition = LEVEL_2;
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ACTIVE_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
            menuIconSelected.setVisible(true);
            menuIconSelected.moveTo(menuIconSelected.getX(), 202);
            modeState.ecoActive = false;
            modeState.boostActive = true;
            modeState.silentActive = false;
            updateBoostIcon();
            updateEcoIcon();
            updateSilentIcon();
        }
    }
    else if (statePlanMenu == SHOWING)
    {
        showMenuTickCounter = 0;
        menuCursorPosition = LEVEL_2;
        selectedPlan = PLAN_WEEK;
        backgroundTopMenuButton.setVisible(false);
        iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_NORMAL_ID));
        iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_NORMAL_ID));
        iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
        iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
        iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
        iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
        planClicked = true;
        removeMenu(statePlanMenu);
        textAreaMenu2.setColor(color_dirty_white);
        menuBackground2.invalidate();
        arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
    }
    else if (stateTimerMenu == SHOWING)
    {
        if (timerState.hours4Active == true)
        {
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            textAreaMenu2.setColor(color_dark_red);
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            menuIconSelected.setVisible(false);
            timerState.hours4Active = false;
            timerState.timerActive = false;
            updateTimerIcon();
        }
        else
        {
            menuCursorPosition = LEVEL_2;
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            menuIconSelected.setVisible(true);
            menuIconSelected.moveTo(menuIconSelected.getX(), 202);
            timerState.hours2Active = false;
            timerState.hours4Active = true;
            timerState.hours6Active = false;
            timerState.timerActive = true;
            menuIconSelected.setVisible(true);
            menuIconSelected.invalidate();
            updateTimerIcon();
        }
    }
    else if (stateSetupMenu == SHOWING)
    {
        if (iconMenu2.getBitmap() == touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ACTIVE_ID))
        {
            menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu2.setColor(color_dark_red);
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ID));
            arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        }
        else
        {
            menuCursorPosition = LEVEL_2;
            selectedSetup = SETUP_TIMER_SETTINGS;
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_COLOR_PICKER_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ACTIVE_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
        }
        arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
    }
    menuBackground2.invalidate();
    menuBackground1.invalidate();
    menuBackground3.invalidate();
    menuBackground4.invalidate();
}

void MainView::menu3Clicked()
{
    //THE FONCTION THIS CALL WHEN THE menu3FlexButton is CLICKED
    // updates when you click on the frist supmenu
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
    textAreaMenu3.setColor(color_dirty_white);
    menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu1.setColor(color_dark_red);
    menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu2.setColor(color_dark_red);
    menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu4.setColor(color_dark_red);

    if (stateModeMenu == SHOWING)
    {
        if (modeState.silentActive == true)
        {
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu3.setColor(color_dark_red);
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
            menuIconSelected.setVisible(false);
            modeState.silentActive = false;
            updateSilentIcon();
        }
        else
        {
            menuCursorPosition = LEVEL_3;
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ACTIVE_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
            menuIconSelected.setVisible(true);
            menuIconSelected.moveTo(menuIconSelected.getX(), 168);
            modeState.ecoActive = false;
            modeState.boostActive = false;
            modeState.silentActive = true;
            updateBoostIcon();
            updateEcoIcon();
            updateSilentIcon();
        }
    }
    else if (statePlanMenu == SHOWING)
    {
        showMenuTickCounter = 0;
        goToIdleStateTimeOutCounter = 0;
        menuCursorPosition = LEVEL_3;
        selectedPlan = PLAN_MONTH;
        backgroundTopMenuButton.setVisible(false);
        iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_NORMAL_ID));
        iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_NORMAL_ID));
        iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
        iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
        iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ACTIVE_ID));
        iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
        planClicked = true;
        removeMenu(statePlanMenu);
        textAreaMenu3.setColor(color_dirty_white);
        menuBackground3.invalidate();
        arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
    }
    else if (stateTimerMenu == SHOWING)
    {
        if (timerState.hours6Active == true)
        {
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu3.setColor(color_dark_red);
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            menuIconSelected.setVisible(false);
            timerState.hours6Active = false;
            timerState.timerActive = false;
            updateTimerIcon();
        }
        else
        {
            menuCursorPosition = LEVEL_3;
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ACTIVE_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            menuIconSelected.setVisible(true);
            menuIconSelected.moveTo(menuIconSelected.getX(), 168);
            timerState.hours2Active = false;
            timerState.hours4Active = false;
            timerState.hours6Active = true;
            timerState.timerActive = true;
            menuIconSelected.setVisible(true);
            menuIconSelected.invalidate();
            updateTimerIcon();
        }
    }
    else if (stateSetupMenu == SHOWING)
    {
        if (iconMenu3.getBitmap() == touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ACTIVE_ID))
        {
            menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu3.setColor(color_dark_red);
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ID));
            arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        }
        else
        {
            menuCursorPosition = LEVEL_3;
            selectedSetup = SETUP_THEMES;
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_COLOR_PICKER_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ACTIVE_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
            menuBackground3.invalidate();
        }
        arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
    }
    menuBackground1.invalidate();
    menuBackground2.invalidate();
    menuBackground3.invalidate();
    menuBackground4.invalidate();
}

void MainView::menu4Clicked()
{
    //THE FONCTION THIS CALL WHEN THE menu4FlexButton is CLICKED
    // updates when you click on the fourth supmenu
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
    textAreaMenu4.setColor(color_dirty_white);
    menuBackground1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu1.setColor(color_dark_red);
    menuBackground2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu2.setColor(color_dark_red);
    menuBackground3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
    textAreaMenu3.setColor(color_dark_red);

    if (stateModeMenu == SHOWING)
    {
        if (iconMenu4.getBitmap() == touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ACTIVE_ID))
        {
            menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu4.setColor(color_dark_red);
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        }
        else
        {
            menuCursorPosition = LEVEL_4;
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_ECO_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_BOOST_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_SILENT_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_FAN_ANGLE_ACTIVE_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
        }
    }
    else if (statePlanMenu == SHOWING)
    {
        if (iconMenu4.getBitmap() == touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID))
        {
            menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu4.setColor(color_dark_red);
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        }
        else
        {
            menuCursorPosition = LEVEL_4;
            selectedPlan = PLAN_NONE;
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CALENDAR_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
        }
    }
    else if (stateTimerMenu == SHOWING)
    {
        if (iconMenu4.getBitmap() == touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID))
        {
            menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu4.setColor(color_dark_red);
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        }
        else
        {
            menuCursorPosition = LEVEL_4;
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_HOURS_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
        }
    }
    else if (stateSetupMenu == SHOWING)
    {
        if (iconMenu4.getBitmap() == touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID))
        {
            menuBackground4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_BUTTON_NORMAL_ID));
            textAreaMenu4.setColor(color_dark_red);
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        }
        else
        {
            menuCursorPosition = LEVEL_4;
            selectedSetup = SETUP_CUSTOMIZE;
            iconMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_COLOR_PICKER_ID));
            iconMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_TIME_SETTINGS_ID));
            iconMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_THEME_ID));
            iconMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ICON_CUSTOM_ACTIVE_ID));
            arrowMenu4.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_ACTIVE_ID));
        }
        arrowMenu1.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu2.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
        arrowMenu3.setBitmap(touchgfx::Bitmap(BITMAP_MENU_ARROW_NORMAL_ID));
    }
    menuBackground1.invalidate();
    menuBackground2.invalidate();
    menuBackground3.invalidate();
    menuBackground4.invalidate();
}

void MainView::buttonDownClicked()
{
    // CLOSE THE MENU OPEN WHEN YOU CLICK ON THE menuDownFlexButton
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    selectedMenu = MENU_NONE;
    if (stateModeMenu == SHOWING)
    {
        menuIconMode.setBitmap(Bitmap(BITMAP_ICON_MODE_NORMAL_ID));
        textAreaMode.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
        removeMenu(stateModeMenu);
    }
    else if (statePlanMenu == SHOWING)
    {
        removeMenu(statePlanMenu);
        menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
        textAreaPlan.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
    }
    else if (stateTimerMenu == SHOWING)
    {
        menuIconTimer.setBitmap(Bitmap(BITMAP_ICON_TIMER_NORMAL_ID));
        textAreaTimer.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
        removeMenu(stateTimerMenu);
    }
    else if (stateSetupMenu == SHOWING)
    {
        menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
        textAreaSetup.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
        removeMenu(stateSetupMenu);
    }
}

void MainView::topBackButtonClicked()
{
    // CLOSE THE PAGE OPEN WHEN YOU CLICK ON THE backTopFlexButton
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    topMenu = TOP_MENU_BACK;
    menuCursorPosition = LEVEL_DOWN_BUTTON;

    if (planDisplay == SHOWING)
    {
        textAreaMenu2.setColor(color_dark_red);
        menuBackground2.invalidate();
        textAreaMenu3.setColor(color_dark_red);
        menuBackground3.invalidate();
        menuIconPlan.setBitmap(Bitmap(BITMAP_ICON_PLAN_NORMAL_ID));
        textAreaPlan.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
        topMenu = TOP_MENU_NONE;
        planClicked = false;
        showMenuTickCounter = 0;
        backgroundTopMenuButton.moveTo(0, 0);
        backgroundTopMenuButton.setVisible(true);
        iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_ACTIVE_ID));
        backgroundTopMenuButton.invalidate();
        planDisplay = ANIMATING_OUT;
        selectedPlan = PLAN_NONE;
    }
    else if (colorPickerScreen == SHOWING)
    {
        menuIconSetup.setBitmap(Bitmap(BITMAP_ICON_SETUP_NORMAL_ID));
        textAreaSetup.setColor(color_golden_yellow);
        boxModeSelected.setVisible(false);
        boxModeSelected.invalidate();
        topMenu = TOP_MENU_NONE;
        colorPickerClicked = false;
        showMenuTickCounter = 0;
        backgroundTopMenuButton.moveTo(0, 0);
        backgroundTopMenuButton.setVisible(true);
        iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_ACTIVE_ID));
        backgroundTopMenuButton.invalidate();
        colorPickerScreen = ANIMATING_OUT;
        selectedSetup = SETUP_NONE;
    }
}

void MainView::topEditButtonClicked()
{
    // CLOSE THE PAGE OPEN WHEN YOU CLICK ON THE editTopFlexButton
    keyTimeOutCounter = 0;
    goToIdleStateTimeOutCounter = 0;
    if (topMenu == TOP_MENU_NONE)
    {
        iconBackTopMenu.setBitmap(Bitmap(BITMAP_ARROW_GO_BACK_TOP_MENU_NORMAL_ID));
        iconBackTopMenu.invalidate();
        backgroundTopMenuButton.moveTo(200, 0);
        backgroundTopMenuButton.setVisible(true);
        iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_ACTIVE_ID));
        backgroundTopMenuButton.invalidate();
        topMenu = TOP_MENU_EDIT;
    }
    else if (topMenu == TOP_MENU_EDIT)
    {
        iconEditTopMenu.setBitmap(Bitmap(BITMAP_EDIT_TOP_MENU_NORMAL_ID));
        backgroundTopMenuButton.invalidate();
        backgroundTopMenuButton.setVisible(false);
        topMenu = TOP_MENU_NONE;
    }
}

void MainView::handleClickEvent(const ClickEvent& event)
{
    MainViewBase::handleClickEvent(event);
    //DISPLAY MENU WHEN CLICK ON THE SPINNING CIRCLE
    if (idleScreen == SHOWING)
    {
        showMenuTickCounter = 0;
        idleScreen = ANIMATING_OUT;
    }
}

void MainView::imageColorPickerClickHandler(const Image& i, const ClickEvent& evt)
{
    // WHEN THE PICKER COLOR IS DISPLAY, put the selecter where you click
    if ((evt.getX() - 104) * (evt.getX() - 104) + (evt.getY() - 104) * (evt.getY() - 104) < 104 * 104)
    {
        // in to the circle
        imageSelecterColorPicker.setX(evt.getX() - 4);
        imageSelecterColorPicker.setY(evt.getY() - 4);
        containerColorPicker.invalidate();
    }
}

void MainView::handleDragEvent(const DragEvent& evt)
{
    // WHEN THE PICKER COLOR IS DISPLAY, draw the selecter
    MainViewBase::handleDragEvent(evt);
    if (colorPickerScreen == SHOWING)
    {
        if ((evt.getNewX() - 120) * (evt.getNewX() - 120) + (evt.getNewY() - 191) * (evt.getNewY() - 191) < 104 * 104)
        {
            // in to the circle
            imageSelecterColorPicker.setX(evt.getNewX() - 20);
            imageSelecterColorPicker.setY(evt.getNewY() - 91);
            containerColorPicker.invalidate();
        }
    }
}
