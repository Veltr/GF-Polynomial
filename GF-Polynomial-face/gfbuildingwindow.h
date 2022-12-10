#ifndef GFBUILDINGWINDOW_H
#define GFBUILDINGWINDOW_H

#include "../GF_polynom/gf.h"

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressBar>
#include <QThread>
#include <QDebug>


class GfBuildingWindow : public QDialog{
	Q_OBJECT
public:
	explicit GfBuildingWindow(GF*, const std::list<int>&, QWidget *parent = nullptr);
	~GfBuildingWindow();
	QString get_result();

//signals:
//	void sending_result();

private slots:
	void cancel();

private:
	friend class Coroutine;
	friend class BuildThread;
	QProgressBar* _bar1;
	QProgressBar* _bar2;
	QTextEdit* _logs_box;
	QPushButton* _stop_it;

	GF* _gf;
	QString _out_string = "";

	QVector<QThread*> _thrs;

	QLayout* get_ui();
	void do_work(const std::list<int>&);
};

class Coroutine : public QThread {
	Q_OBJECT
public:
	Coroutine(GfBuildingWindow& w) : _w(w) {};
	void shutdown(){ _is_sh = true; };

signals:
	void send1(int);
	void send2(int);
	void send3(QString);

private:
	bool _is_sh = false;
	GfBuildingWindow& _w;
	void run();
};

class BuildThread : public QThread {
	Q_OBJECT
public:
	BuildThread(GfBuildingWindow& w, const std::list<int>& list) : _w(w), _list(list) {};

signals:
	void error();

private:
	GfBuildingWindow& _w;
	std::list<int> _list;
	void run();
};

#endif // GFBUILDINGWINDOW_H
