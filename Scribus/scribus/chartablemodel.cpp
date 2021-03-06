/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QItemSelectionModel>

#include "fonts/scface.h"
#include "scribusdoc.h"
#include "scpainter.h"
#include "chartablemodel.h"


CharTableModel::CharTableModel(QObject *parent, int cols, ScribusDoc * doc, const QString & font)
		: QAbstractTableModel(parent),
		m_doc(doc),
		m_cols(cols),
		m_viewWidth(200),
		m_fontInUse(font)
{
	m_selectionModel = new QItemSelectionModel(this);
	m_characters.clear();
}

int CharTableModel::rowCount(const QModelIndex & parent) const
{
	return m_doc || !parent.isValid() ? m_characters.count() / m_cols + 1: 0;
}

int CharTableModel::columnCount(const QModelIndex & /* parent */) const
{
	return m_cols;
}

QVariant CharTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || m_doc == 0)
		return QVariant();

	int ix = index.row() * m_cols + index.column();
	int currentChar;
	if (ix < m_characters.count())
		currentChar = m_characters[ix];
	else
		return QVariant();

	// for mimeData()
	if (role == Qt::AccessibleTextRole)
		return m_characters[ix];

	// tooltip
	if (role == Qt::ToolTipRole)
	{
		QString tmp;
		tmp.sprintf("%04X", currentChar);
		return "Unicode:\n0x"+tmp;
	}

	// pixmap
	if (role == Qt::DecorationRole)
	{
		// m_cols should not become 0. Never.
		int baseSize = m_viewWidth / m_cols;
		QTransform chma;
		chma.scale(baseSize/10, baseSize/10);

		ScFace face = (*m_doc->AllFonts)[m_fontInUse];
		uint gl = face.char2CMap(currentChar);
		int size = baseSize + qRound(-face.descent() * baseSize) + 1;
		double ww = baseSize - face.glyphWidth(gl, baseSize);

		QImage pix(baseSize, size, QImage::Format_ARGB32_Premultiplied);
		ScPainter *p = new ScPainter(&pix, baseSize, size);
		p->clear();
		FPointArray gly = face.glyphOutline(gl, 1);
		if (gly.size() > 4)
		{
			gly.map(chma);
			p->translate(ww / 2, 1);
			p->setBrush(Qt::black);
			p->setFillMode(1);
			p->setupPolygon(&gly);
			p->fillPath();
			p->end();
		}
		delete p;
		return QVariant(QPixmap::fromImage(pix));
	}
	// trash
	return QVariant();
}

void CharTableModel::setDoc(ScribusDoc *doc)
{
	// repaint only when doc differs
	if (doc != m_doc)
	{
		m_doc = doc;
		reset();
	}
}

ScFace CharTableModel::fontFace()
{
	return (*m_doc->AllFonts)[m_fontInUse];
}

void CharTableModel::setCharacters(CharClassDef ch)
{
	m_characters.clear();
	m_characters = ch;
	reset();
}

void CharTableModel::setFontInUse(QString font)
{
	if (font != m_fontInUse)
	{
		m_fontInUse = font;
		reset();
	}
}

void CharTableModel::appendUnicode(const QString & s, uint base)
{
	int orig = rowCount();
	bool ok;
	int val = s.toInt(&ok, base);
	if (!ok)
		return;

	if (!m_characters.contains(val))
	{
		m_characters.append(val);
		reset();
	}
	else
	{
		// select already contained glyph
		int ix = m_characters.indexOf(val);
		int ixrow = ix / m_cols;
		int ixcol = ix % m_cols;
		m_selectionModel->select(index(ixrow, ixcol, QModelIndex()), QItemSelectionModel::ClearAndSelect);
		emit selectionChanged(m_selectionModel);
	}

	if (orig < rowCount())
		emit rowAppended();
}

bool CharTableModel::removeCharacter(int index)
{
	if (index >= 0 && index < m_characters.size())
	{
		m_characters.removeAt(index);
		reset();
		return true;
	}
//	qDebug("CharTable::deleteOwnCharacter: no char deleted - logical error probably");
	return false;
}

Qt::ItemFlags CharTableModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
	if (index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions CharTableModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QStringList CharTableModel::mimeTypes() const
{
	return QStringList() << "text/plain";
}

QMimeData *CharTableModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	mimeData->setText(data(indexes.at(0), Qt::AccessibleTextRole).toString());
	return mimeData;
}

bool CharTableModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
	if (action == Qt::IgnoreAction)
		return true;

	if (!data->hasText())
		return false;

	appendUnicode(QString(data->text()), 10);
	return true;
}
