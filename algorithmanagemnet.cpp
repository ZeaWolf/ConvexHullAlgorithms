#include "covexhullwindow.h"

int RNG(int a, int b) {
	int range = b - a;
	int rn = (rand() % range) + a;
	return rn;
};

void GenerateInitialPoints(std::vector<D2D1_POINT_2F>* vec, int size) {
	for (int i = 0; i < size; i++) {
		vec->push_back( D2D1::Point2F((float)RNG(LOWRAND, UPPRAND), (float)RNG(LOWRAND, UPPRAND)) );
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

void MainWindow::DrawPoint(int code) {
	if (code == 1)
	{
		for (auto i = ellipses1.begin(); i != ellipses1.end(); ++i)
			(*i)->Draw(pRenderTargetM, pBrushM);
	}
	else if (code == 2)
	{
		for (auto j = ellipses2.begin(); j != ellipses2.end(); ++j)
			(*j)->Draw(pRenderTargetM, pBrushM);
	}
	else if (code == 3)
	{
		for (auto i = ellipses1.begin(); i != ellipses1.end(); ++i)
			(*i)->Draw(pRenderTargetM, pBrushM);
		for (auto j = ellipses2.begin(); j != ellipses2.end(); ++j)
			(*j)->Draw(pRenderTargetM, pBrushM);
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

void MainWindow::ShowQuickhull() {
	DrawGraph(pRenderTargetM, pBrushM);

	Func::DoQuickhull(&Qraw, &Qresult);


	DrawPolygon(pRenderTargetM, pBrushM, &Qresult, D2D1::ColorF(D2D1::ColorF::Maroon));

	// cirle
	DrawPoint(1);

	if (Selection1())
	{
		pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		pRenderTargetM->DrawEllipse(Selection1()->ellipse, pBrushM, 2.0f);
		pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
		pRenderTargetM->FillEllipse(Selection1()->ellipse, pBrushM);
	}

	if (Selection2())
	{
		pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		pRenderTargetM->DrawEllipse(Selection2()->ellipse, pBrushM, 2.0f);
		pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
		pRenderTargetM->FillEllipse(Selection2()->ellipse, pBrushM);
	}
}