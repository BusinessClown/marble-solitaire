#include "main_window.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSize>
#include <QStackedWidget>
#include <QString>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVector>

#include "tic_tac_toe_widget.h"

namespace {

// One selectable tile on the marble solitaire setup page.
struct TileOption {
  const char* icon_path;
  const char* label;
  int id;
};

// Board shapes the player can choose. The icons are Qt resources; add the
// PNG files to a .qrc file with the prefix "/assets" so that they are
// compiled into the binary and work on any machine.
constexpr TileOption kBoardOptions[] = {
    {":/assets/french.png", "French", 0},
    {":/assets/jcwiegleb.png", "J.C. Wiegleb", 1},
    {":/assets/asymmetrical.png", "Asymmetrical", 2},
    {":/assets/english.png", "English", 3},
    {":/assets/diamond.png", "Diamond", 4},
    {":/assets/triangle.png", "Triangle", 5},
};

// Where the single empty hole starts.
constexpr TileOption kStartPositionOptions[] = {
    {":/assets/standard.png", "Standard", 0},
    {":/assets/random.png", "Random", 1},
};

constexpr int kWindowWidth = 450;
constexpr int kWindowHeight = 550;
constexpr int kBoardIconSide = 128;
constexpr int kStartIconSide = 64;

constexpr char kTileStyleSheet[] =
    "QToolButton { border: 2px solid transparent; border-radius: 8px;"
    " padding: 6px; }"
    "QToolButton:checked { border: 2px solid #3399ff;"
    " background: #eef6ff; }";

// Builds one checkable icon tile and registers it with `group`.
QToolButton* CreateTile(const TileOption& option, int icon_side,
                        QButtonGroup* group, QWidget* parent) {
  QToolButton* button = new QToolButton(parent);
  button->setCheckable(true);
  button->setIcon(QIcon(QString::fromUtf8(option.icon_path)));
  button->setIconSize(QSize(icon_side, icon_side));
  button->setText(QString::fromUtf8(option.label));
  button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  button->setAutoRaise(true);
  button->setStyleSheet(QString::fromUtf8(kTileStyleSheet));
  group->addButton(button, option.id);
  return button;
}

}  // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle(tr("Qt Test App"));
  resize(kWindowWidth, kWindowHeight);

  stacked_widget_ = new QStackedWidget(this);
  setCentralWidget(stacked_widget_);

  // The order of these calls must match the values of Page.
  stacked_widget_->addWidget(CreateMainPage());
  stacked_widget_->addWidget(CreateChooseFirstPlayerPage());
  stacked_widget_->addWidget(CreateTicTacToePage());
  stacked_widget_->addWidget(CreateMarbleBoardPage());

  ShowPage(Page::kMain);

  qDebug() << "MainWindow constructed. Qt version:" << qVersion();
}

MainWindow::~MainWindow() = default;

QWidget* MainWindow::CreateMainPage() {
  QWidget* page = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout(page);

  click_label_ = new QLabel(tr("Qt is working!"), page);
  click_label_->setAlignment(Qt::AlignCenter);
  click_label_->setStyleSheet("font-size: 18px;");

  click_button_ = new QPushButton(tr("Click me"), page);
  check_box_ = new QCheckBox(tr("Enable something"), page);

  radio_a_ = new QRadioButton(tr("Option A"), page);
  radio_b_ = new QRadioButton(tr("Option B"), page);
  radio_a_->setChecked(true);

  // A button group keeps the two radio buttons mutually exclusive. Giving
  // it `page` as a parent hands its lifetime to Qt.
  QButtonGroup* radio_group = new QButtonGroup(page);
  radio_group->addButton(radio_a_);
  radio_group->addButton(radio_b_);

  QHBoxLayout* radio_layout = new QHBoxLayout;
  radio_layout->addWidget(radio_a_);
  radio_layout->addWidget(radio_b_);

  status_label_ = new QLabel(tr("Status: Option A selected"), page);
  status_label_->setAlignment(Qt::AlignCenter);

  play_tic_tac_toe_button_ = new QPushButton(tr("Play Tic-Tac-Toe"), page);
  play_tic_tac_toe_button_->setStyleSheet("font-size: 14px; padding: 8px;");

  play_marble_button_ =
      new QPushButton(tr("Start Marble Solitaire Game"), page);
  play_marble_button_->setStyleSheet("font-size: 14px; padding: 8px;");

  layout->addWidget(click_label_);
  layout->addWidget(click_button_);
  layout->addWidget(check_box_);
  layout->addLayout(radio_layout);
  layout->addWidget(status_label_);
  layout->addStretch();
  layout->addWidget(play_tic_tac_toe_button_);
  layout->addWidget(play_marble_button_);

  connect(click_button_, &QPushButton::clicked, this,
          &MainWindow::OnClickButtonClicked);
  connect(check_box_, &QCheckBox::toggled, this,
          &MainWindow::OnCheckBoxToggled);
  connect(radio_a_, &QRadioButton::clicked, this,
          &MainWindow::OnRadioSelected);
  connect(radio_b_, &QRadioButton::clicked, this,
          &MainWindow::OnRadioSelected);
  connect(play_tic_tac_toe_button_, &QPushButton::clicked, this,
          &MainWindow::ShowChooseFirstPlayerPage);
  connect(play_marble_button_, &QPushButton::clicked, this,
          &MainWindow::ShowMarbleBoardPage);

  return page;
}

QWidget* MainWindow::CreateChooseFirstPlayerPage() {
  QWidget* page = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout(page);

  QLabel* title = new QLabel(tr("Who goes first?"), page);
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet("font-size: 22px; font-weight: bold;");

  choose_x_button_ = new QPushButton(tr("X goes first"), page);
  choose_o_button_ = new QPushButton(tr("O goes first"), page);
  choose_x_button_->setStyleSheet("font-size: 16px; padding: 12px;");
  choose_o_button_->setStyleSheet("font-size: 16px; padding: 12px;");

  // The stretches above and below centre the content vertically.
  layout->addStretch();
  layout->addWidget(title);
  layout->addSpacing(20);
  layout->addWidget(choose_x_button_);
  layout->addWidget(choose_o_button_);
  layout->addStretch();

  connect(choose_x_button_, &QPushButton::clicked, this,
          &MainWindow::StartGameWithX);
  connect(choose_o_button_, &QPushButton::clicked, this,
          &MainWindow::StartGameWithO);

  return page;
}

QWidget* MainWindow::CreateTicTacToePage() {
  QWidget* page = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout(page);

  QLabel* title = new QLabel(tr("Tic-Tac-Toe"), page);
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet("font-size: 20px; font-weight: bold;");

  tic_tac_toe_ = new TicTacToeWidget(page);

  game_status_label_ = new QLabel(tr("X's turn"), page);
  game_status_label_->setAlignment(Qt::AlignCenter);
  game_status_label_->setStyleSheet(
      "font-size: 16px; font-weight: bold; padding: 6px;");

  game_back_button_ = new QPushButton(tr("Back"), page);
  game_reset_button_ = new QPushButton(tr("Reset Board"), page);

  QHBoxLayout* button_layout = new QHBoxLayout;
  button_layout->addWidget(game_back_button_);
  button_layout->addWidget(game_reset_button_);

  layout->addWidget(title);
  layout->addWidget(tic_tac_toe_);
  layout->addWidget(game_status_label_);
  layout->addLayout(button_layout);

  connect(game_back_button_, &QPushButton::clicked, this,
          &MainWindow::ShowMainPage);

  // A mid-game reset always gives the first move back to X.
  connect(game_reset_button_, &QPushButton::clicked, this,
          [this]() { tic_tac_toe_->ResetBoard(true); });

  connect(tic_tac_toe_, &TicTacToeWidget::StatusChanged, game_status_label_,
          &QLabel::setText);

  return page;
}

QWidget* MainWindow::CreateMarbleBoardPage() {
  QWidget* page = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout(page);

  QLabel* title = new QLabel(tr("Board and Position Selection"), page);
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet("font-size: 22px; font-weight: bold;");

  QLabel* board_label = new QLabel(tr("Choose a board:"), page);
  board_label->setAlignment(Qt::AlignCenter);

  board_group_ = new QButtonGroup(page);
  board_group_->setExclusive(true);

  QHBoxLayout* board_layout = new QHBoxLayout;
  for (const TileOption& option : kBoardOptions) {
    board_layout->addWidget(
        CreateTile(option, kBoardIconSide, board_group_, page));
  }
  board_group_->button(0)->setChecked(true);

  QLabel* start_label = new QLabel(tr("Starting position:"), page);
  start_label->setAlignment(Qt::AlignCenter);

  start_position_group_ = new QButtonGroup(page);
  start_position_group_->setExclusive(true);

  QHBoxLayout* start_layout = new QHBoxLayout;
  for (const TileOption& option : kStartPositionOptions) {
    start_layout->addWidget(
        CreateTile(option, kStartIconSide, start_position_group_, page));
  }
  start_position_group_->button(0)->setChecked(true);

  marble_back_button_ = new QPushButton(tr("Back"), page);
  QHBoxLayout* button_layout = new QHBoxLayout;
  button_layout->addWidget(marble_back_button_);

  layout->addWidget(title);
  layout->addSpacing(10);
  layout->addWidget(board_label);
  layout->addLayout(board_layout);
  layout->addSpacing(20);
  layout->addWidget(start_label);
  layout->addLayout(start_layout);
  layout->addSpacing(20);
  layout->addLayout(button_layout);
  layout->addStretch();

  connect(marble_back_button_, &QPushButton::clicked, this,
          &MainWindow::ShowMainPage);

  return page;
}

void MainWindow::ShowPage(Page page) {
  stacked_widget_->setCurrentIndex(static_cast<int>(page));
  qDebug() << "Switched to page" << static_cast<int>(page);
}

void MainWindow::ShowMainPage() { ShowPage(Page::kMain); }

void MainWindow::ShowChooseFirstPlayerPage() {
  ShowPage(Page::kChooseFirstPlayer);
}

void MainWindow::ShowMarbleBoardPage() { ShowPage(Page::kMarbleBoard); }

void MainWindow::StartGameWithX() {
  tic_tac_toe_->ResetBoard(true);
  ShowPage(Page::kTicTacToe);
}

void MainWindow::StartGameWithO() {
  tic_tac_toe_->ResetBoard(false);
  ShowPage(Page::kTicTacToe);
}

void MainWindow::OnClickButtonClicked() {
  ++click_count_;
  click_label_->setText(tr("Button clicked %1 time(s)").arg(click_count_));
}

void MainWindow::OnCheckBoxToggled(bool checked) {
  status_label_->setText(checked ? tr("Status: Checkbox ON")
                                 : tr("Status: Checkbox OFF"));
}

void MainWindow::OnRadioSelected() {
  const QString selected =
      radio_a_->isChecked() ? tr("Option A") : tr("Option B");
  status_label_->setText(tr("Status: %1 selected").arg(selected));
}
