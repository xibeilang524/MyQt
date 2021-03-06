﻿#include "widget.h"
#include "ui_widget.h"

#include <QWebView>
#include <QWebFrame>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    initMap();
}

void Widget::initMap()
{
    QString mapPath = "file:///" + QApplication::applicationDirPath();
    ui->webView->load(mapPath + "/map.html");
    ui->webView->setContentsMargins(0,0,0,0);
    ui->webView->setTextSizeMultiplier(0.8);

    connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(slotPopulateJavaScriptWindowObject()));
}

void Widget::addMarker(const QString &lot, const QString &lat,
                       const QString &micon, const QString &name,
                       const QString &mmsi)
{
    QWebFrame *frame = ui->webView->page()->mainFrame();
    QString marker = QString("addMarker(\"%1\",\"%2\",\"%3\",\"%4\",\"%5\")").
            arg(lot).arg(lat).arg(micon).arg(name).arg(mmsi);
    frame->evaluateJavaScript(marker);
}

void Widget::bmapOneMarker(const QString &lot, const QString &lat,
                           const QString &msg, const QString &img)
{
    QWebFrame *frame = ui->webView->page()->mainFrame();
    QString mk = QString("BMapOneMarker(\"%1\",\"%2\",\"%3\",\"%4\")")
            .arg(lot).arg(lat).arg(msg).arg(img);
    frame->evaluateJavaScript(mk);
}

void Widget::addCircle(const QString &lot, const QString &lat, const int radius)
{
    QWebFrame *frame = ui->webView->page()->mainFrame();
    QString cmd = QString("addCircle(\"%1\",\"%2\",\"%3\")").arg(lot).arg(lat).arg(radius);
    frame->evaluateJavaScript(cmd);
}

void Widget::getCoordinate(const QString &lot, const QString &lat)
{
    qDebug()<<"lot:"<<lot<<" lat:"<<lat;
}

void Widget::slotPopulateJavaScriptWindowObject()
{
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("ReinforcePC", this);
}

void Widget::on_pbnAddMarker_clicked()
{
    QString lot = ui->lineEdit_lot->text();
    QString lat = ui->lineEdit_lat->text();
    QString name = ui->lineEdit_name->text();
    QString mmsi = ui->lineEdit_mmsi->text();

    //addMarker(lot,lat,"images/ship_mark.png",name,mmsi);

    bmapOneMarker(lot,lat,name,mmsi);
}

void Widget::on_pbnSetCircle_clicked()
{
    //QPoint point(116.498605,39.931929);
    //bmapOneMarker("116.498605","39.931929","sss","images/ship_mark.png");

    int radius = ui->lineEdit_radius->text().toInt();
    addCircle("116.498278","39.922965",radius);
}
