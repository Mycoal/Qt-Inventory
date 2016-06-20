#include <QPainter>
#include <QModelIndex>
#include "bardelegate.h"

BarDelegate::BarDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}


QWidget *BarDelegate::createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const
{
 return 0;
}


void BarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index,Qt::EditRole).toInt();

    QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
    spinbox->setValue(value);
}

void BarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
    spinbox->interpretText();
    int value = spinbox->value();
    model->setData(index,value, Qt::EditRole);
}



void BarDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}


void BarDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{


  int value = index.model()->data( index, Qt::DisplayRole ).toInt();
  double factor = double(value)/100.0;

  painter->save();

  if( factor > 1 )
  {
    painter->setBrush( Qt::red );
    factor = 1;
  }
  else
    painter->setBrush( QColor( 0, int(factor*255), 255-int(factor*255) ) );

  painter->setPen( Qt::black );
  painter->drawRect( option.rect.x()+2, option.rect.y()+2, int(factor*(option.rect.width()-5)), option.rect.height()-5 );
  painter->restore();
}
QSize BarDelegate::sizeHint( const QStyleOptionViewItem &, const QModelIndex & ) const
{
  return QSize( 45, 15 );
}
