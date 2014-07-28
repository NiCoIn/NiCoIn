#include "nicoinunits.h"

#include <QStringList>

NiCoInUnits::NiCoInUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<NiCoInUnits::Unit> NiCoInUnits::availableUnits()
{
    QList<NiCoInUnits::Unit> unitlist;
    unitlist.append(NCI);
    unitlist.append(mNCI);
    unitlist.append(uNCI);
    return unitlist;
}

bool NiCoInUnits::valid(int unit)
{
    switch(unit)
    {
    case NCI:
    case mNCI:
    case uNCI:
        return true;
    default:
        return false;
    }
}

QString NiCoInUnits::name(int unit)
{
    switch(unit)
    {
    case NCI: return QString("NCI");
    case mNCI: return QString("mNCI");
    case uNCI: return QString::fromUtf8("μNCI");
    default: return QString("???");
    }
}

QString NiCoInUnits::description(int unit)
{
    switch(unit)
    {
    case NCI: return QString("NiCoIns");
    case mNCI: return QString("Milli-NiCoIns (1 / 1,000)");
    case uNCI: return QString("Micro-NiCoIns (1 / 1,000,000)");
    default: return QString("???");
    }
}

mpq NiCoInUnits::factor(int unit)
{
    switch(unit)
    {
    case uNCI: return mpq("100/1");
    case mNCI: return mpq("100000/1");
    default:
    case NCI:  return mpq("100000000/1");
    }
}

int NiCoInUnits::amountDigits(int unit)
{
    switch(unit)
    {
    case NCI: return 8; // <100,000,000 (# digits, without commas)
    case mNCI: return 11; // <100,000,000,000
    case uNCI: return 14; // <100,000,000,000,000
    default: return 0;
    }
}

int NiCoInUnits::decimals(int unit)
{
    switch(unit)
    {
    case NCI: return 8;
    case mNCI: return 5;
    case uNCI: return 2;
    default: return 0;
    }
}

QString NiCoInUnits::format(int unit, const mpq& n, bool fPlus)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    mpq q = n * COIN / factor(unit);
    std::string str = FormatMoney(q, fPlus);
    int diff = 8 - decimals(unit);
    if(diff > 0)
        str.erase(str.length() - diff, diff);
    return QString::fromStdString(str);
}

QString NiCoInUnits::formatWithUnit(int unit, const mpq& amount, bool plussign)
{
    return format(unit, amount, plussign) + QString(" ") + name(unit);
}

bool NiCoInUnits::parse(int unit, const QString &value, mpq *val_out)
{
    mpq ret_value;
    if (!ParseMoney(value.toStdString(), ret_value))
        return false;
    if(val_out)
    {
        *val_out = ret_value * factor(unit) / COIN;
    }
    return true;
}

int NiCoInUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant NiCoInUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}
