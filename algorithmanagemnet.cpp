#include "covexhullwindow.h"

int RNG(int a, int b) {
	int range = b - a;
	int rn = (rand() % range) + a;
	return rn;
};

void GenerateInitialPoints(std::vector<D2D1_POINT_2F>* vec, int size, int lowrand, int upprand) {
	for (int i = 0; i < size; i++) {
		vec->push_back( D2D1::Point2F((float)RNG(lowrand, upprand), (float)RNG(lowrand, upprand)) );
	}
	return;
};



void SortPoints(std::vector<D2D1_POINT_2F>* vec) {
	std::vector<float> angle;
	std::vector<D2D1_POINT_2F> temp;
	float xc = 0, yc = 0;
	for (unsigned int i = 0; i < vec->size(); i++) {
		xc += (*vec)[i].x;
		yc += (*vec)[i].y;
		temp.push_back((*vec)[i]);
	}
	xc = xc / vec->size();
	yc = yc / vec->size();

	for (unsigned int i = 0; i < vec->size(); i++) {
		angle.push_back((float)atan2( (*vec)[i].x-xc, (*vec)[i].y - yc ));
	}
	vec->clear();

	unsigned int count = 0;
	unsigned int index;
	float rad;
	while (count < temp.size()) {
		index = -1;
		rad = -10.0;
		for (unsigned int i = 0; i < angle.size(); i++)
		{
			if (angle[i] > rad)
			{
				index = i;
				rad = angle[i];
			}
		}
		vec->push_back(temp[index]);
		angle[index] = -10.0;
		count++;
	}
	return;
}

void MainWindow::RefreshPoints() {
	switch (mode)
	{
	case InitialMode:
		break;

	case MinkowskiDifferenceMode:
		ellipses1.clear();
		ellipses2.clear();
		InsertPoints(&MDraw1, &MDraw2);
		break;

	case MinkowskiSumMode:
		ellipses1.clear();
		ellipses2.clear();
		InsertPoints(&MSraw1, &MSraw2);
		break;

	case QuickhullMode:
		ellipses1.clear();
		ellipses2.clear();
		InsertPoints(&Qraw, NULL);
		break;

	case PointConvexHullMode:
		ellipses1.clear();
		ellipses2.clear();
		//Func::DoQuickhull(&PCraw, &PCconvex);
		//SortPoints(&PCconvex)
		InsertPoints(&PCtarget, NULL); /////
		break;

	case GJKMode:
		ellipses1.clear();
		ellipses2.clear();
		InsertPoints(&GJKraw1, &GJKraw2);
		break;
	}
}

void MainWindow::DrawPoint(int code) {
	if (code == 1)
	{
		for (auto i = ellipses1.begin(); i != ellipses1.end(); ++i)
		{
			//(*i)->color = color;
			(*i)->Draw(pRenderTargetM, pBrushM);
		}
	}
	else if (code == 2)
	{
		for (auto j = ellipses2.begin(); j != ellipses2.end(); ++j)
		{
			//(*j)->color = color;
			(*j)->Draw(pRenderTargetM, pBrushM);
		}
	}
	else if (code == 3)
	{
		for (auto i = ellipses1.begin(); i != ellipses1.end(); ++i)
		{
			//(*i)->color = color;
			(*i)->Draw(pRenderTargetM, pBrushM);
		}
		for (auto j = ellipses2.begin(); j != ellipses2.end(); ++j)
		{
			//(*j)->color = color;
			(*j)->Draw(pRenderTargetM, pBrushM);
		}
	}

	return;
}

void MainWindow::DrawPolygon(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pB, std::vector<D2D1_POINT_2F>* vec, D2D1_COLOR_F color)
{
	pB->SetColor(color);
	int count = 0;
	D2D1_POINT_2F* p1 = NULL,* p2=NULL,* p0=NULL;
	for (unsigned int i = 0; i < vec->size(); i++)
	{
		p2 = &(*vec)[i];
		if (p1)
		{
			pRT->DrawLine( D2D1::Point2F((float)ConvertXToDip(p1->x), (float)ConvertYToDip(p1->y)), 
				D2D1::Point2F((float)ConvertXToDip(p2->x), (float)ConvertYToDip(p2->y)),
				pB, 3.0F);
		}
		p1 = &(*vec)[i];
		if (!p0)
		{
			p0 = &(*vec)[i];
		}
	}

	if (p0 != p1)
	{
		pRT->DrawLine(D2D1::Point2F((float)ConvertXToDip(p2->x), (float)ConvertYToDip(p2->y)),
			D2D1::Point2F((float)ConvertXToDip(p0->x), (float)ConvertYToDip(p0->y)),
			pB, 3.0F);
	}
	return;
};


void MainWindow::UpdatePoint(std::vector<D2D1_POINT_2F>* vec, float x, float y, float newx, float newy) {
	for (unsigned int i = 0; i < vec->size(); i++) {
		if ((*vec)[i].x == x && (*vec)[i].y == y) {
			(*vec)[i] = D2D1::Point2F(newx, newy);
			break;
		}
	}
	return;
}

void MainWindow::ClearSelection3() { SelectPoly = NULL; Displacement.clear(); };

void MainWindow::SetSelectPoly(std::vector<D2D1_POINT_2F>* vec, D2D1_POINT_2F pt) {
	SelectPoly = vec;
	//float x = ConvertDipToX(pt.x);
	//float y = ConvertDipToY(pt.y);
	for(unsigned int i = 0; i < vec->size(); i++)
	{
		Displacement.push_back(D2D1::Point2F((*vec)[i].x - pt.x, (*vec)[i].y - pt.y));
	}
}

void MainWindow::UpdatePoly(D2D1_POINT_2F pt)
{
	float x = ConvertDipToX(pt.x);
	float y = ConvertDipToY(pt.y);
	for (unsigned int i = 0; i < SelectPoly->size(); i++) {
		(*SelectPoly)[i].x = Displacement[i].x + x;
		(*SelectPoly)[i].y = Displacement[i].y + y;
	}
	return;
}

void MainWindow::ShowQuickhull() {

	Qresult.clear();
	//DrawGraph(pRenderTargetM, pBrushM);
	Func::DoQuickhull(&Qraw, &Qresult);
	DrawPolygon(pRenderTargetM, pBrushM, &Qresult, D2D1::ColorF(D2D1::ColorF::Maroon));

	// cirle
	DrawPoint(1);

	//Select Point color change
	SelectedPointColor();
}


void MainWindow::ShowPointConvexHull() {
	//DrawGraph(pRenderTargetM, pBrushM);
	DrawPolygon(pRenderTargetM, pBrushM, &PCconvex, D2D1::ColorF(D2D1::ColorF::Maroon));
	DrawPoint(3);
	
}

void MainWindow::ShowMinkowskiSum() {
	MSconvex1.clear();
	MSconvex2.clear();
	MSresult.clear();
	std::vector<D2D1_POINT_2F>	temp;
	DrawGraph(pRenderTargetM, pBrushM);
	Func::DoQuickhull(&MSraw1, &MSconvex1);
	Func::DoQuickhull(&MSraw2, &MSconvex2);
	Func::DoMinkowskiSum(&MSraw1, &MSraw2, &temp);
	Func::DoQuickhull(&temp, &MSresult);

	DrawPolygon(pRenderTargetM, pBrushM, &MSresult, D2D1::ColorF(D2D1::ColorF::Purple));
	DrawPolygon(pRenderTargetM, pBrushM, &MSconvex2, D2D1::ColorF(D2D1::ColorF::SteelBlue));
	DrawPolygon(pRenderTargetM, pBrushM, &MSconvex1, D2D1::ColorF(D2D1::ColorF::Maroon));

	// cirle
	DrawPoint(3);

	//Select Point color change
	SelectedPointColor();
}

void MainWindow::ShowMinkowskiDifference() {

	MDconvex1.clear();
	MDconvex2.clear();
	MDresult.clear();
	std::vector<D2D1_POINT_2F>	temp;
	DrawGraph(pRenderTargetM, pBrushM);
	Func::DoQuickhull(&MDraw1, &MDconvex1);
	Func::DoQuickhull(&MDraw2, &MDconvex2);
	Func::DoMinkowskiDiff(&MDraw1, &MDraw2, &temp);
	Func::DoQuickhull(&temp, &MDresult);
	DrawPolygon(pRenderTargetM, pBrushM, &MDresult, D2D1::ColorF(D2D1::ColorF::Purple));
	DrawPolygon(pRenderTargetM, pBrushM, &MDconvex2, D2D1::ColorF(D2D1::ColorF::SteelBlue));
	DrawPolygon(pRenderTargetM, pBrushM, &MDconvex1, D2D1::ColorF(D2D1::ColorF::Maroon));
	// cirle
	DrawPoint(3);

	//Select Point color change
	SelectedPointColor();
}

void MainWindow::ShowGJK() {
	GJKconvex1.clear();
	GJKconvex2.clear();
	GJKresult.clear();
	std::vector<D2D1_POINT_2F>	temp;
	DrawGraph(pRenderTargetM, pBrushM);
	Func::DoQuickhull(&GJKraw1, &GJKconvex1);
	Func::DoQuickhull(&GJKraw2, &GJKconvex2);

	bool col = Func::GJK(&GJKraw1, &GJKraw2, &temp);
	Func::DoQuickhull(&temp, &GJKresult);


	DrawPolygon(pRenderTargetM, pBrushM, &GJKresult, col? D2D1::ColorF(D2D1::ColorF::DarkCyan) : D2D1::ColorF(D2D1::ColorF::Purple));
	DrawPolygon(pRenderTargetM, pBrushM, &GJKconvex2, D2D1::ColorF(D2D1::ColorF::SteelBlue));
	DrawPolygon(pRenderTargetM, pBrushM, &GJKconvex1, D2D1::ColorF(D2D1::ColorF::Maroon));

	DrawPoint(3);

	//Select Point color change
	SelectedPointColor();
}





void MainWindow::SelectedPointColor() {
	if (Selection1())
	{
		pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
		pRenderTargetM->DrawEllipse(Selection1()->ellipse, pBrushM, 2.0f);
		pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		pRenderTargetM->FillEllipse(Selection1()->ellipse, pBrushM);
	}

	if (Selection2())
	{
		pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
		pRenderTargetM->DrawEllipse(Selection2()->ellipse, pBrushM, 2.0f);
		pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		pRenderTargetM->FillEllipse(Selection2()->ellipse, pBrushM);
	}
}


