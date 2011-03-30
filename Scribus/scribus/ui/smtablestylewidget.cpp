/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smtablestylewidget.h"
#include "util_icon.h"

SMTableStyleWidget::SMTableStyleWidget(QWidget *parent) : QWidget()
{
	setupUi(this);

	backgroundColorIcon->setPixmap(loadIcon("16/color-fill.png"));
	backgroundColor->addItem(CommonStrings::tr_NoneColor);
}

SMTableStyleWidget::~SMTableStyleWidget()
{
}


void SMTableStyleWidget::show(TableStyle *tableStyle, QList<TableStyle> &tableStyles, const QString &defLang, int unitIndex)
{
	Q_ASSERT(tableStyle);
	if (!tableStyle)
		return;

	// TODO: Handle parent styles (and language?)

	backgroundColor->setCurrentText(tableStyle->backgroundColor());
}

void SMTableStyleWidget::show(QList<TableStyle*> &tableStyles, QList<TableStyle> &tableStylesAll, const QString &defaultLanguage, int unitIndex)
{
	if (tableStyles.count() == 1) {
		show(tableStyles[0], tableStylesAll, defaultLanguage, unitIndex);
	}

	// TODO: Implement actual support multiple styles.
}

void SMTableStyleWidget::languageChange()
{
	backgroundColor->setToolTip(tr("Background Color"));
}

void SMTableStyleWidget::fillBackgroundColorCombo(ColorList &colors)
{
	backgroundColor->clear();

	backgroundColor->addItem(CommonStrings::tr_NoneColor);
	ColorList::Iterator itEnd = colors.end();
	ScribusDoc* doc = colors.document();
	for (ColorList::Iterator it = colors.begin(); it != itEnd; ++it)
	{
		backgroundColor->insertFancyItem(it.value(), doc, it.key());
	}
	backgroundColor->view()->setMinimumWidth(backgroundColor->view()->maximumViewportSize().width()+24);
}


