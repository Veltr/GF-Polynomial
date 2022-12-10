#include "mainwindow.h"
#include <sstream>
#include <map>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
	auto w1 = new QWidget(); w1->setLayout(get_pol_ui());
	auto w2 = new QWidget(); w2->setLayout(get_gf_ui());
	auto w3 = new QWidget(); w3->setLayout(get_px_ui());

	auto sp = new QSplitter(Qt::Horizontal);
	sp->setStyleSheet("QSplitter::handle{background: #999999;}"
					  "QSplitter::handle:vertical{height: 3px;}");

	auto sp2 = new QSplitter(Qt::Horizontal);
	sp2->setStyleSheet("QSplitter::handle{background: #999999;}"
					  "QSplitter::handle:vertical{height: 3px;}");

	/*sp->addWidget(w1);
	sp->addWidget(w3);
	sp->addWidget(w2);*/

	sp2->addWidget(w1);
	sp2->addWidget(w3);

	auto h1 = new QHBoxLayout();
	h1->addWidget(new QLabel("p1"));
	h1->addWidget(_p1 = new QLineEdit());
	_p1->setPlaceholderText("Example: {1, 2 3}x^3 + (2a^2 + 1)x^2 + a^3 x");

	auto v1 = new QVBoxLayout();
	v1->addLayout(h1);
	v1->addWidget(sp2);

	auto w4 = new QWidget(); w4->setLayout(v1);

	//sp->addWidget(sp2);
	sp->addWidget(w4);
	sp->addWidget(w2);

	setCentralWidget(sp);
	setWindowTitle("Полиномы над GF");
}

MainWindow::~MainWindow(){
	if(_gf) delete _gf;
}

QList<int> parse_line(const QString& str, char c = 'x'){
	QRegExp rx(QString("(\\d*)(?:%1)((?:\\^)(\\d+))?|(\\d+)").arg(c));
	int pos = 0;
	std::map<int, int> ps; // p, k

	while ((pos = rx.indexIn(str, pos)) != -1) { // 5x^6 + 1 => 5 - cap(1); 6 - cap(3); 1 - cap(4)
		int k = rx.cap(1) == "" ? (rx.cap(4) == "" ? 1 : rx.cap(4).toInt()) : rx.cap(1).toInt();
		int p = rx.cap(3) == "" ? (rx.cap(4) == "" ? 1 : 0) : rx.cap(3).toInt();

		if(auto i = ps.find(p); i != ps.end()) i->second += k;
		else ps[p] = k;

		pos += rx.matchedLength();
	}
	if(!ps.size()) return {};
	auto i = ps.rbegin();
	int size = i->first + 1;
	QVector<int> out(size, 0);
	for(; i != ps.rend(); i++) out[size - i->first - 1] = i->second;

	return out.toList();
}

PolynomGF parse_pol(const QString& str, GF* gf){
	QRegExp rx("(?:(?:(\\{[^\\}]+\\})|(\\([^\\)]+\\))|(a\\^\\d+))(?:(\\s*x(?:(?:\\^(\\d*))|(?:.)|(?:\\s*$)))|(?:\\s+)|(?:\\s*$)))");
	QRegExp rx1_1("(?:(\\d+)(?:(?:\\s+)|(?:\\S+\\s*)))"); // {...}
	QRegExp rx1_2("(?:a\\^)(\\d+)"); // a^
	std::map<int, GF::Element> map;
	int pos = 0;

	while ((pos = rx.indexIn(str, pos)) != -1) { // {..}-c1; (..)-c2, a^-c3, x-c4, ^x'a-c5
		//list << rx.cap(1) << rx.cap(2) << rx.cap(3) << rx.cap(4) << rx.cap(5);

		QString t; int p1 = 0; GF::Element e;
		if(t = rx.cap(1); t != ""){
			QList<int> l;
			while ((p1 = rx1_1.indexIn(t, p1)) != -1) {
				l.push_back(rx1_1.cap(1).toInt());
				p1 += rx1_1.matchedLength();
			}
			auto lt = std::list<int>(l.begin(), l.end());
			//out.append(GF::Element(lt, gf));
			e = GF::Element(lt, gf);
		}
		else if(t = rx.cap(2); t != ""){
			auto l = parse_line(t, 'a');
			auto lt = std::list<int>(l.begin(), l.end());
			//out.append(GF::Element(lt, gf));
			e = GF::Element(lt, gf);
		}
		else if(t = rx.cap(3); t != ""){
			if((p1 = rx1_2.indexIn(t, p1)) != -1){
				int d = rx1_2.cap(1).toInt();
				//out.append(gf->get_element_by_power(d));
				e = gf->get_element_by_power(d);
			}
		}

		int p = rx.cap(4) == "" ? 0 : (rx.cap(5) == "" ? 1 : rx.cap(5).toInt());
		map[p] = e;

		pos += rx.matchedLength();
	}

	if(!map.size()) return nullptr;

	return PolynomGF(gf, map);
}

void MainWindow::build_gf(){
	/*_gf = new GF(_mode->value(), _power->value());
	auto t = parse_line(_pol->text());
	try { _gf->build_GF(std::list<int>(t.begin(), t.end()));}
	catch (const std::exception& e) {
		_gf_text->setPlainText(QString::fromStdString(e.what()));
		return;
	}

	//auto f = std::async(std::launch::async, [&]{_gf->build_GF(std::list<int>(t.begin(), t.end()));});

	std::ostringstream s;

	//f.get();
	_gf->print_GF(s);
	_gf_text->setPlainText(QString::fromStdString(s.str()));*/

	//_p1->setText("sas");
//	auto w = new GfBuildingWindow(_gf, this);
//	//w->show();
//	//connect(w, &GfBuildingWindow::sending_result, this, &MainWindow::end_building_gf);
//	connect(w, &GfBuildingWindow::sending_result, this, [&]{ _p2->setText(w->get_result()); });
//	w->exec();
//	//_gf_text->setPlainText(QString::number(w->result()));
//	delete w;
//	//_p2->setText("yeee");

	_gf = new GF(_mode->value(), _power->value());
	auto t = parse_line(_pol->text());

	auto w = new GfBuildingWindow(_gf, std::list<int>(t.begin(), t.end()), this);
	//connect(w, &GfBuildingWindow::sending_result, this, [&]{ _p2->setText(w->get_result()); });
	if(w->Rejected == w->exec()){
		delete _gf;
		_gf_text->setPlainText("Canceled\n" + w->get_result());
	}
	else{
		std::ostringstream s;

		_gf->print_GF(s);
		_gf_text->setPlainText(QString::fromStdString(s.str()));
	}
	delete w;
}

void MainWindow::calc_px(){
	PolynomGF p1 = parse_pol(_p1->text(), _gf);
	GF::Element px = (parse_pol(_px->text(), _gf)).get_by_power(0);
//	PolynomGF p2 = parse_pol(_px->text(), _gf);
//	GF::Element px = p2.get_by_power(0);

	std::ostringstream s;
	s << "p = " << p1 << "\nx = " << px << "\np(x) = " << p1.calc_value(px);
	//s << p1.calc_value(px);
	_px_out->setPlainText(QString::fromStdString(s.str()));
}

void MainWindow::do_action(PolynomGF(PolynomGF::*action)(const PolynomGF&)){
	//parse_pol("{41, 2, 5}x^5 + (5a^4 + a^3 + 2a + 1)x^3 + (a + 12)", _gf);
	//parse_pol("{41, 2, 5,,,, 6 7}");

	PolynomGF p1 = parse_pol(_p1->text(), _gf);
	std::ostringstream s; s << "p1 =  " << p1 << '\n';

	PolynomGF p2 = parse_pol(_p2->text(), _gf);
	s << "p2 =  " << p2 << '\n';

	QPushButton* send = (QPushButton*)sender();
	if(send->text() != "/"){
		auto pr = (p1.*action)(p2);
		s << "pr =  " << pr << '\n';
	}
	else{
		auto[k, r] = p1.div_with_rem(p2);
		s << "div = " << k << "\nrem = " << r << '\n';
	}

	_pr->setPlainText(QString::fromStdString(s.str()));
}

QLayout* MainWindow::get_gf_ui(){
	auto out = new QVBoxLayout();

	auto g = new QGridLayout();

	auto h1 = new QHBoxLayout();
	h1->addWidget(new QLabel("Mode"));
	h1->addWidget(_mode = new QSpinBox());
	_mode->setMinimum(2);
	_mode->setMaximum(100000);

	auto h2 = new QHBoxLayout();
	h2->addWidget(new QLabel("Power"));
	h2->addWidget(_power = new QSpinBox());
	_power->setMinimum(1);

	auto h3 = new QHBoxLayout();
	h3->addWidget(new QLabel("Polynom"));
	h3->addWidget(_pol = new QLineEdit());
	_pol->setPlaceholderText("x^4 + 5x^3 + 2x + 1");

	g->addLayout(h1, 0, 0);
	g->addLayout(h2, 0, 1);
	//g->addLayout(h3, 1, 0, 1, 2);
	g->addLayout(h3, 1, 0);
	auto b1 = new QPushButton("Build Gf");
	connect(b1, &QPushButton::clicked, this, &MainWindow::build_gf);
	g->addWidget(b1, 1, 1);


	_gf_text = new QTextEdit;

	out->addLayout(g);
	out->addWidget(_gf_text);

	return out;
}

QLayout* MainWindow::get_pol_ui(){
	auto out = new QVBoxLayout();

//	auto h1 = new QHBoxLayout();
//	h1->addWidget(new QLabel("p1"));
//	h1->addWidget(_p1 = new QLineEdit());
//	_p1->setPlaceholderText("Example: {1, 2 3}x^3 + (2a^2 + 1)x^2 + a^3 x");

	auto h2 = new QHBoxLayout();
	h2->addWidget(new QLabel("p2"));
	h2->addWidget(_p2 = new QLineEdit());

	auto h3 = new QHBoxLayout();
	h3->addWidget(new QLabel("pr"));
	h3->addWidget(_pr = new QTextEdit());
	//h3->addWidget(new QLineEdit());


	//PolynomGF (PolynomGF::*f)(const PolynomGF&) = &PolynomGF::operator*;

	auto hb = new QHBoxLayout();
	auto b1 = new QPushButton("+");
	connect(b1, &QPushButton::clicked, this, [&]{ do_action(&PolynomGF::operator+); });
	hb->addWidget(b1);

	auto b2 = new QPushButton("-");
	connect(b2, &QPushButton::clicked, this, [&]{ do_action(&PolynomGF::operator-); });
	hb->addWidget(b2);

	auto b3 = new QPushButton("*");
	connect(b3, &QPushButton::clicked, this, [&]{ do_action(&PolynomGF::operator*); });
	hb->addWidget(b3);

	auto b4 = new QPushButton("/");
	connect(b4, &QPushButton::clicked, this, [&]{ do_action(&PolynomGF::operator/); });
	hb->addWidget(b4);

	//out->addLayout(h1);
	out->addLayout(h2);
	out->addLayout(hb);
	out->addLayout(h3);

	return out;
}

QLayout* MainWindow::get_px_ui(){
	auto out = new QVBoxLayout();

	auto h1 = new QHBoxLayout();
	h1->addWidget(new QLabel("x"));
	h1->addWidget(_px = new QLineEdit());

	auto h2 = new QHBoxLayout();
	h2->addWidget(new QLabel("p1(x)"));
	h2->addWidget(_px_out = new QTextEdit());
	auto b4 = new QPushButton("Calc p1(x)");
	connect(b4, &QPushButton::clicked, this, &MainWindow::calc_px);
	h2->addWidget(b4);

	out->addLayout(h1);
	out->addLayout(h2);
	return out;
}

