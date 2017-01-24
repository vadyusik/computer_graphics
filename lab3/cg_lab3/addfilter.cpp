#include "addfilter.h"
#include "ui_addfilter.h"

addfilter::addfilter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addfilter)
{
    ui->setupUi(this);
}

addfilter::~addfilter()
{
    delete ui;
}
