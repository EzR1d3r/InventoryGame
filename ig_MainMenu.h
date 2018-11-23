#pragma once
#include <QWidget>
#include <ig_consts.h>

namespace Ui
{
	class ig_MainMenu;
}

class IG_MainMenu : public QWidget
{
	Q_OBJECT
public:
	explicit IG_MainMenu(QWidget *parent = nullptr);
	~IG_MainMenu();
Q_SIGNALS:
	void close_mm(bool);
	void new_game(NetworkRole);
	void exit_game(int i);
protected:
	void closeEvent(QCloseEvent *event);
private Q_SLOTS:
	void on_btnNewGame_clicked();
	void on_btnExit_clicked();
private:
	Ui::ig_MainMenu *ui;
};
