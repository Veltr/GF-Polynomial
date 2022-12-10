#include "gfbuildingwindow.h"

GfBuildingWindow::GfBuildingWindow(GF* gf, const std::list<int>& list, QWidget *parent) : QDialog(parent, Qt::Window){
	setLayout(get_ui());
//	connect(this, &GfBuildingWindow::rejected, this, &GfBuildingWindow::sending_result);
//	connect(this, &GfBuildingWindow::accepted, this, &GfBuildingWindow::sending_result);

	connect(_stop_it, &QPushButton::clicked, this, &GfBuildingWindow::cancel);

	_gf = gf;
	do_work(list);
}

GfBuildingWindow::~GfBuildingWindow(){
	((Coroutine*)_thrs[0])->shutdown();
	for(int i = 0; i <_thrs.size(); i++)
		if(_thrs[i] != nullptr && _thrs[i]->isRunning()){
			//_thrs[i]->quit();
			//_thrs[i]->wait();
			_thrs[i]->terminate();
		}
}

QString GfBuildingWindow::get_result(){
	return _out_string;
}

void GfBuildingWindow::cancel(){
	if(_thrs[1]->isRunning()){
		reject();
	}
	else accept();
}

QLayout* GfBuildingWindow::get_ui(){
	auto v1 = new QVBoxLayout();

	v1->addWidget(_bar1 = new QProgressBar());
	v1->addWidget(_bar2 = new QProgressBar());

	v1->addWidget(_logs_box = new QTextEdit());
	v1->addWidget(_stop_it = new QPushButton("Stop"));
	connect(_stop_it, &QPushButton::clicked, this, &GfBuildingWindow::cancel);

	return v1;
}



void GfBuildingWindow::do_work(const std::list<int>& list){
	/*std::thread corot([&]{
		float last = -1;
		while(1){
			auto cur = _gf->get_build_progress();
			if(last != cur.first){
				_logs_box->append(QString::fromStdString(cur.msg));
				last = cur.first;
			}
			_bar1->setValue(cur.first * 100);
			_bar2->setValue(cur.second * 100);

			if(cur.first == 1 && cur.second == 1) return;
		}
	});*/

	/*std::thread thr([&]{
		_gf->build_GF(list);
	});*/

	auto t1 = new BuildThread(*this, list);
	auto corot = new Coroutine(*this);
	_thrs.append(corot);
	_thrs.append(t1);

	//connect(corot, &QThread::finished, corot, &QObject::deleteLater);

	corot->start();
	t1->start();
}


void Coroutine::run(){
	float last = -1;
	connect(this, &Coroutine::send1, _w._bar1, &QProgressBar::setValue);
	connect(this, &Coroutine::send2, _w._bar2, &QProgressBar::setValue);
	connect(this, &Coroutine::send3, _w._logs_box, &QTextEdit::append);
	while(!_is_sh){
		auto cur = _w._gf->get_build_progress();
		if(last != cur.first){
			emit send3(QString::fromStdString(cur.msg));
			last = cur.first;
		}
		emit send1(cur.first * 100);
		emit send2(cur.second * 100);

		QThread::msleep(10);
	}
}

void BuildThread::run(){
	connect(this, &BuildThread::error, &_w, &GfBuildingWindow::reject);
	connect(this, &BuildThread::finished, &_w, &GfBuildingWindow::accept);
	try {
		_w._gf->build_GF(_list);
		//_w._logs_box->append("I'm done");
	}  catch (const std::exception& e) {
		_w._out_string = e.what();
		emit error();
		QThread::msleep(100);
	}
}
