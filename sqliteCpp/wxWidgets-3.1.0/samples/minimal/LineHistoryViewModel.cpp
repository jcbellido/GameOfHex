#include "LineHistoryViewModel.h"

#include "wx/log.h"

MyMusicTreeModel::MyMusicTreeModel()
{
	m_root = new MyMusicTreeModelNode(NULL, "My Music");

	// setup pop music
	m_pop = new MyMusicTreeModelNode(m_root, "Pop music");
	m_pop->Append(
		new MyMusicTreeModelNode(m_pop, "You are not alone", "Michael Jackson", 1995));
	m_pop->Append(
		new MyMusicTreeModelNode(m_pop, "Take a bow", "Madonna", 1994));
	m_root->Append(m_pop);

	// setup classical music
	m_classical = new MyMusicTreeModelNode(m_root, "Classical music");
	m_ninth = new MyMusicTreeModelNode(m_classical, "Ninth symphony",
		"Ludwig van Beethoven", 1824);
	m_classical->Append(m_ninth);
	m_classical->Append(new MyMusicTreeModelNode(m_classical, "German Requiem",
		"Johannes Brahms", 1868));
	m_root->Append(m_classical);

	m_classicalMusicIsKnownToControl = false;
}

wxString MyMusicTreeModel::GetTitle(const wxDataViewItem &item) const
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	if (!node)      // happens if item.IsOk()==false
		return wxEmptyString;

	return node->m_title;
}

wxString MyMusicTreeModel::GetArtist(const wxDataViewItem &item) const
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	if (!node)      // happens if item.IsOk()==false
		return wxEmptyString;

	return node->m_artist;
}

int MyMusicTreeModel::GetYear(const wxDataViewItem &item) const
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	if (!node)      // happens if item.IsOk()==false
		return 2000;

	return node->m_year;
}

void MyMusicTreeModel::AddToClassical(const wxString &title, const wxString &artist, unsigned int year)

{
	if (!m_classical)
	{
		wxASSERT(m_root);

		// it was removed: restore it
		m_classical = new MyMusicTreeModelNode(m_root, "Classical music");
		m_root->Append(m_classical);

		// notify control
		wxDataViewItem child((void*)m_classical);
		wxDataViewItem parent((void*)m_root);
		ItemAdded(parent, child);
	}

	// add to the classical music node a new node:
	MyMusicTreeModelNode *child_node =
		new MyMusicTreeModelNode(m_classical, title, artist, year);
	m_classical->Append(child_node);

	// FIXME: what's m_classicalMusicIsKnownToControl for?
	if (m_classicalMusicIsKnownToControl)
	{
		// notify control
		wxDataViewItem child((void*)child_node);
		wxDataViewItem parent((void*)m_classical);
		ItemAdded(parent, child);
	}
}

void MyMusicTreeModel::Delete(const wxDataViewItem &item)
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	if (!node)      // happens if item.IsOk()==false
		return;

	wxDataViewItem parent(node->GetParent());
	if (!parent.IsOk())
	{
		wxASSERT(node == m_root);

		// don't make the control completely empty:
		wxLogError("Cannot remove the root item!");
		return;
	}

	// is the node one of those we keep stored in special pointers?
	if (node == m_pop)
		m_pop = NULL;
	else if (node == m_classical)
		m_classical = NULL;
	else if (node == m_ninth)
		m_ninth = NULL;

	// first remove the node from the parent's array of children;
	// NOTE: MyMusicTreeModelNodePtrArray is only an array of _pointers_
	//       thus removing the node from it doesn't result in freeing it
	node->GetParent()->GetChildren().Remove(node);

	// free the node
	delete node;

	// notify control
	ItemDeleted(parent, item);
}

int MyMusicTreeModel::Compare(const wxDataViewItem &item1, const wxDataViewItem &item2,
	unsigned int column, bool ascending) const
{
	wxASSERT(item1.IsOk() && item2.IsOk());
	// should never happen

	if (IsContainer(item1) && IsContainer(item2))
	{
		wxVariant value1, value2;
		GetValue(value1, item1, 0);
		GetValue(value2, item2, 0);

		wxString str1 = value1.GetString();
		wxString str2 = value2.GetString();
		int res = str1.Cmp(str2);
		if (res) return res;

		// items must be different
		wxUIntPtr litem1 = (wxUIntPtr)item1.GetID();
		wxUIntPtr litem2 = (wxUIntPtr)item2.GetID();

		return litem1 - litem2;
	}

	return wxDataViewModel::Compare(item1, item2, column, ascending);
}

void MyMusicTreeModel::GetValue(wxVariant &variant,
	const wxDataViewItem &item, unsigned int col) const
{
	wxASSERT(item.IsOk());

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	switch (col)
	{
	case 0:
		variant = node->m_title;
		break;
	case 1:
		variant = node->m_artist;
		break;
	case 2:
		variant = (long)node->m_year;
		break;
	case 3:
		variant = node->m_quality;
		break;
	case 4:
		variant = 80L;  // all music is very 80% popular
		break;
	case 5:
		if (GetYear(item) < 1900)
			variant = "old";
		else
			variant = "new";
		break;

	default:
		wxLogError("MyMusicTreeModel::GetValue: wrong column %d", col);
	}
}

bool MyMusicTreeModel::SetValue(const wxVariant &variant,
	const wxDataViewItem &item, unsigned int col)
{
	wxASSERT(item.IsOk());

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	switch (col)
	{
	case 0:
		node->m_title = variant.GetString();
		return true;
	case 1:
		node->m_artist = variant.GetString();
		return true;
	case 2:
		node->m_year = variant.GetLong();
		return true;
	case 3:
		node->m_quality = variant.GetString();
		return true;

	default:
		wxLogError("MyMusicTreeModel::SetValue: wrong column");
	}
	return false;
}

bool MyMusicTreeModel::IsEnabled(const wxDataViewItem &item,
	unsigned int col) const
{
	wxASSERT(item.IsOk());

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();

	// disable Beethoven's ratings, his pieces can only be good
	return !(col == 3 && node->m_artist.EndsWith("Beethoven"));
}

wxDataViewItem MyMusicTreeModel::GetParent(const wxDataViewItem &item) const
{
	// the invisible root node has no parent
	if (!item.IsOk())
		return wxDataViewItem(0);

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();

	// "MyMusic" also has no parent
	if (node == m_root)
		return wxDataViewItem(0);

	return wxDataViewItem((void*)node->GetParent());
}

bool MyMusicTreeModel::IsContainer(const wxDataViewItem &item) const
{
	// the invisble root node can have children
	// (in our model always "MyMusic")
	if (!item.IsOk())
		return true;

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	return node->IsContainer();
}

unsigned int MyMusicTreeModel::GetChildren(const wxDataViewItem &parent,
	wxDataViewItemArray &array) const
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)parent.GetID();
	if (!node)
	{
		array.Add(wxDataViewItem((void*)m_root));
		return 1;
	}

	if (node == m_classical)
	{
		MyMusicTreeModel *model = (MyMusicTreeModel*)(const MyMusicTreeModel*) this;
		model->m_classicalMusicIsKnownToControl = true;
	}

	if (node->GetChildCount() == 0)
	{
		return 0;
	}

	unsigned int count = node->GetChildren().GetCount();
	for (unsigned int pos = 0; pos < count; pos++)
	{
		MyMusicTreeModelNode *child = node->GetChildren().Item(pos);
		array.Add(wxDataViewItem((void*)child));
	}

	return count;
}

