/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "commonstrings.h"
#include "util_math.h"
#include "tablestyle.h"

QString TableStyle::displayName() const
{
	if (isDefaultStyle())
		return CommonStrings::trDefaultCharacterStyle;
	if (hasName() || !hasParent() || !m_context)
		return name();
	else 
		return parentStyle()->displayName() + "+";
}

bool TableStyle::equiv(const Style& other) const
{
	other.validate();
	const TableStyle *p_other = dynamic_cast<const TableStyle*> (&other);
	return p_other &&
		parent() == p_other->parent() 
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		&& (inh_##attr_NAME == p_other->inh_##attr_NAME) \
		&& (inh_##attr_NAME || isequiv(m_##attr_NAME, p_other->m_##attr_NAME))
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
		;
}

void TableStyle::erase()
{
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME) \
		reset##attr_NAME();
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	//updateFeatures(); TODO: Investigate this.
}

