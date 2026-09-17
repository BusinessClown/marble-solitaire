#include "tic_tac_toe_game.h"

#include <gtest/gtest.h>

namespace tictactoe {
namespace {

// Test 1: a reset board is empty and the requested player moves first.
TEST(TicTacToeGameTest, ResetClearsBoardAndSetsFirstPlayer) {
  Game game;
  game.PlayMove(0, 0);

  game.Reset(Player::kO);

  EXPECT_EQ(game.current_player(), Player::kO);
  EXPECT_EQ(game.state(), GameState::kInProgress);
  EXPECT_FALSE(game.is_over());
  for (int row = 0; row < Game::kBoardSize; ++row) {
    for (int col = 0; col < Game::kBoardSize; ++col) {
      EXPECT_EQ(game.CellAt(row, col), Cell::kEmpty)
          << "cell (" << row << ", " << col << ") should be empty";
    }
  }
}

// Test 2: illegal moves are rejected and do not consume a turn.
TEST(TicTacToeGameTest, RejectsOccupiedAndOffBoardMoves) {
  Game game;

  ASSERT_TRUE(game.PlayMove(1, 1));
  EXPECT_EQ(game.CellAt(1, 1), Cell::kX);
  EXPECT_EQ(game.current_player(), Player::kO);

  // O tries to take the cell X just used.
  EXPECT_FALSE(game.PlayMove(1, 1));
  EXPECT_EQ(game.CellAt(1, 1), Cell::kX);
  EXPECT_EQ(game.current_player(), Player::kO);

  // Coordinates outside the board are ignored too.
  EXPECT_FALSE(game.PlayMove(3, 0));
  EXPECT_FALSE(game.PlayMove(-1, 2));
  EXPECT_EQ(game.current_player(), Player::kO);
}

// Test 3: three in a row ends the game and records the winning line.
TEST(TicTacToeGameTest, DetectsDiagonalWinAndRecordsWinningLine) {
  Game game;

  game.PlayMove(0, 0);  // X
  game.PlayMove(0, 1);  // O
  game.PlayMove(1, 1);  // X
  game.PlayMove(0, 2);  // O
  game.PlayMove(2, 2);  // X wins on the main diagonal.

  EXPECT_EQ(game.state(), GameState::kXWins);
  EXPECT_TRUE(game.is_over());
  EXPECT_EQ(game.StatusText(), "X wins!");

  const auto& line = game.winning_line();
  EXPECT_EQ(line[0].row, 0);
  EXPECT_EQ(line[0].col, 0);
  EXPECT_EQ(line[1].row, 1);
  EXPECT_EQ(line[1].col, 1);
  EXPECT_EQ(line[2].row, 2);
  EXPECT_EQ(line[2].col, 2);

  // No further moves are accepted once the game is over.
  EXPECT_FALSE(game.PlayMove(1, 0));
}

// Test 4: a full board with no line is a draw.
TEST(TicTacToeGameTest, FullBoardWithoutWinnerIsADraw) {
  Game game;

  // X O X
  // X O O
  // O X X
  game.PlayMove(0, 0);  // X
  game.PlayMove(0, 1);  // O
  game.PlayMove(0, 2);  // X
  game.PlayMove(1, 1);  // O
  game.PlayMove(1, 0);  // X
  game.PlayMove(2, 0);  // O
  game.PlayMove(2, 1);  // X
  game.PlayMove(1, 2);  // O
  game.PlayMove(2, 2);  // X fills the last cell.

  EXPECT_EQ(game.state(), GameState::kDraw);
  EXPECT_TRUE(game.is_over());
  EXPECT_EQ(game.StatusText(), "It's a draw!");
}

}  // namespace
}  // namespace tictactoe
