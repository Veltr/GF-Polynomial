#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../GF_polynom/gf.h"
#include "../GF_polynom/polynomgf.h"
#include "gfbuildingwindow.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QRegExp>
#include <QSplitter>
#include <QVector>
#include <QMap>
#include <thread>
#include <future>


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void build_gf();
	void calc_px();

private:
	GF* _gf = NULL;

	QSpinBox* _mode;
	QSpinBox* _power;
	QLineEdit* _pol;
	QTextEdit* _gf_text;

	QLineEdit* _p1;
	QLineEdit* _p2;
	QTextEdit* _pr;

	QLineEdit* _px;
	QTextEdit* _px_out;


	void do_action(PolynomGF(PolynomGF::*)(const PolynomGF&));
	QLayout* get_gf_ui();
	QLayout* get_pol_ui();
	QLayout* get_px_ui();
};
#endif // MAINWINDOW_H
