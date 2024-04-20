#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <touchgfx/Color.hpp>

class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void upPressed();
    virtual void downPressed();
    virtual void rightPressed();
    virtual void leftPressed();
    virtual void selectPressed();
    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& event);
    virtual void handleDragEvent(const DragEvent& event);

    void imageColorPickerClickHandler(const Image& i, const ClickEvent& e);

protected:

    Callback<MainView, const Image&, const ClickEvent&> imageColorPickerClickedCallback;

    uint32_t setTemperature;

    struct
    {
        bool boostActive;
        bool ecoActive;
        bool silentActive;
        bool angleActive;
    } modeState;

    struct
    {
        bool hours2Active;
        bool hours4Active;
        bool hours6Active;
        bool timerActive;
    } timerState;

    enum SelectedMenu
    {
        MENU_NONE,
        MENU_MODE,
        MENU_PLAN,
        MENU_TIMER,
        MENU_SETUP
    } selectedMenu;

    enum TopMenu
    {
        TOP_MENU_NONE,
        TOP_MENU_BACK,
        TOP_MENU_EDIT
    } topMenu;

    enum ShowingModeMenu
    {
        NOT_SHOWING,
        ANIMATING_IN,
        SHOWING,
        ANIMATING_OUT
    } stateModeMenu, statePlanMenu, stateTimerMenu, stateSetupMenu, planDisplay, colorPickerScreen, idleScreen;

    enum PlanMode
    {
        PLAN_NONE,
        PLAN_DAY,
        PLAN_WEEK,
        PLAN_MONTH,
        PLAN_CUSTOM
    } selectedPlan;

    enum SetupMode
    {
        SETUP_NONE,
        SETUP_COLOR_PICKER,
        SETUP_TIMER_SETTINGS,
        SETUP_THEMES,
        SETUP_CUSTOMIZE
    } selectedSetup;

    enum MenuCursorPosition
    {
        LEVEL_1,
        LEVEL_2,
        LEVEL_3,
        LEVEL_4,
        LEVEL_DOWN_BUTTON,
    } menuCursorPosition;

    uint32_t showMenuTickCounter;
    uint32_t keyTimeOutCounter;
    uint32_t goToIdleStateTimeOutCounter;

    bool planClicked;
    bool colorPickerClicked;

    void updateTimerIcon();
    void updateBoostIcon();
    void updateEcoIcon();
    void updateSilentIcon();
    void updateAngleIcon();
    void removeMenu(ShowingModeMenu& menu);
    void showMenuMode();
    void showMenuPlan();
    void showMenuTimer();
    void showMenuSetup();
    int16_t NumberSetupMenu;
    bool setupMenuSetupClick;
    bool setupMenuTimerClick;
    bool setupMenuPlanClick;
    bool setupMenuModeClick;
    void setUpTemperature();
    void setDownTemperature();
    void menu1Clicked();
    void menu2Clicked();
    void menu3Clicked();
    void menu4Clicked();
    void buttonDownClicked();
    void topBackButtonClicked();
    void topEditButtonClicked();

    touchgfx::colortype   color_golden_yellow;
    touchgfx::colortype   color_dark_red;
    touchgfx::colortype   color_dirty_white;
    touchgfx::colortype   color_orange;


};

#endif // MAINVIEW_HPP
