#ifndef MARBLE_SOLITAIRE_SRC_MAIN_WINDOW_H_
#define MARBLE_SOLITAIRE_SRC_MAIN_WINDOW_H_

#include <QMainWindow>

class QButtonGroup;
class QCheckBox;
class QLabel;
class QPushButton;
class QRadioButton;
class QStackedWidget;
class QWidget;

class TicTacToeWidget;

// Top-level application window.
//
// The window owns a QStackedWidget holding one page per screen; see Page for
// the order in which they are added. Every child widget is parented, so Qt
// destroys them along with the window and no manual cleanup is needed.
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

 private:
  // Index of each page inside stacked_widget_, in the order the pages are
  // added by the constructor.
  enum class Page {
    kMain = 0,
    kChooseFirstPlayer = 1,
    kTicTacToe = 2,
    kMarbleBoard = 3,
  };

 private slots:
  // Main page.
  void OnClickButtonClicked();
  void OnCheckBoxToggled(bool checked);
  void OnRadioSelected();

  // Navigation.
  void ShowMainPage();
  void ShowChooseFirstPlayerPage();
  void ShowMarbleBoardPage();

  // "Who goes first?" page.
  void StartGameWithX();
  void StartGameWithO();

 private:
  // Page builders. Each is called once from the constructor and returns a
  // widget that the stacked widget takes ownership of.
  QWidget* CreateMainPage();
  QWidget* CreateChooseFirstPlayerPage();
  QWidget* CreateTicTacToePage();
  QWidget* CreateMarbleBoardPage();

  // Shows one page and logs the transition.
  void ShowPage(Page page);

  QStackedWidget* stacked_widget_ = nullptr;

  // Main page.
  QLabel* click_label_ = nullptr;
  QLabel* status_label_ = nullptr;
  QPushButton* click_button_ = nullptr;
  QCheckBox* check_box_ = nullptr;
  QRadioButton* radio_a_ = nullptr;
  QRadioButton* radio_b_ = nullptr;
  QPushButton* play_tic_tac_toe_button_ = nullptr;
  QPushButton* play_marble_button_ = nullptr;
  int click_count_ = 0;

  // "Who goes first?" page.
  QPushButton* choose_x_button_ = nullptr;
  QPushButton* choose_o_button_ = nullptr;

  // Tic-tac-toe page.
  TicTacToeWidget* tic_tac_toe_ = nullptr;
  QLabel* game_status_label_ = nullptr;
  QPushButton* game_back_button_ = nullptr;
  QPushButton* game_reset_button_ = nullptr;

  // Marble solitaire setup page.
  QButtonGroup* board_group_ = nullptr;
  QButtonGroup* start_position_group_ = nullptr;
  QPushButton* marble_back_button_ = nullptr;
};

#endif  // MARBLE_SOLITAIRE_SRC_MAIN_WINDOW_H_
