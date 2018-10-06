#pragma once
#include <QWidget>

namespace Ui
{
	class ig_MainMenu;
}

class IG_MainMenu : public QWidget
{
	Q_OBJECT
public:
	explicit IG_MainMenu(QWidget *parent = 0);
	~IG_MainMenu();
private slots:
	void on_btnNewGame_clicked();
	void on_btnExit_clicked();
signals:
	void new_game();
	void exit_game(int i);
private:
	Ui::ig_MainMenu *ui;
};
