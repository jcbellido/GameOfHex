#pragma once

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "wx/dataview.h"
#include "wx/hashmap.h"

WX_DECLARE_HASH_MAP(unsigned, wxString, wxIntegerHash, wxIntegerEqual,
	IntToStringMap);

// ----------------------------------------------------------------------------
// MyMusicTreeModelNode: a node inside MyMusicTreeModel
// ----------------------------------------------------------------------------

class MyMusicTreeModelNode;
WX_DEFINE_ARRAY_PTR(MyMusicTreeModelNode*, MyMusicTreeModelNodePtrArray);

class MyMusicTreeModelNode
{
public:
	MyMusicTreeModelNode(MyMusicTreeModelNode* parent,
		const wxString &title, const wxString &artist,
		unsigned int year)
	{
		m_parent = parent;

		m_title = title;
		m_artist = artist;
		m_year = year;
		m_quality = "good";

		m_container = false;
	}

	MyMusicTreeModelNode(MyMusicTreeModelNode* parent,
		const wxString &branch)
	{
		m_parent = parent;

		m_title = branch;
		m_year = -1;

		m_container = true;
	}

	~MyMusicTreeModelNode()
	{
		// free all our children nodes
		size_t count = m_children.GetCount();
		for (size_t i = 0; i < count; i++)
		{
			MyMusicTreeModelNode *child = m_children[i];
			delete child;
		}
	}

	bool IsContainer() const
	{
		return m_container;
	}

	MyMusicTreeModelNode* GetParent()
	{
		return m_parent;
	}
	MyMusicTreeModelNodePtrArray& GetChildren()
	{
		return m_children;
	}
	MyMusicTreeModelNode* GetNthChild(unsigned int n)
	{
		return m_children.Item(n);
	}
	void Insert(MyMusicTreeModelNode* child, unsigned int n)
	{
		m_children.Insert(child, n);
	}
	void Append(MyMusicTreeModelNode* child)
	{
		m_children.Add(child);
	}
	unsigned int GetChildCount() const
	{
		return m_children.GetCount();
	}

public:     // public to avoid getters/setters
	wxString                m_title;
	wxString                m_artist;
	int                     m_year;
	wxString                m_quality;

	// TODO/FIXME:
	// the GTK version of wxDVC (in particular wxDataViewCtrlInternal::ItemAdded)
	// needs to know in advance if a node is or _will be_ a container.
	// Thus implementing:
	//   bool IsContainer() const
	//    { return m_children.GetCount()>0; }
	// doesn't work with wxGTK when MyMusicTreeModel::AddToClassical is called
	// AND the classical node was removed (a new node temporary without children
	// would be added to the control)
	bool m_container;

private:
	MyMusicTreeModelNode          *m_parent;
	MyMusicTreeModelNodePtrArray   m_children;
};


// ----------------------------------------------------------------------------
// MyMusicTreeModel
// ----------------------------------------------------------------------------

/*
Implement this data model
Title               Artist               Year        Judgement
--------------------------------------------------------------------------
1: My Music:
2:  Pop music
3:  You are not alone   Michael Jackson      1995        good
4:  Take a bow          Madonna              1994        good
5:  Classical music
6:  Ninth Symphony      Ludwig v. Beethoven  1824        good
7:  German Requiem      Johannes Brahms      1868        good
*/

class MyMusicTreeModel : public wxDataViewModel
{
public:
	MyMusicTreeModel();
	~MyMusicTreeModel()
	{
		delete m_root;
	}

	// helper method for wxLog

	wxString GetTitle(const wxDataViewItem &item) const;
	wxString GetArtist(const wxDataViewItem &item) const;
	int GetYear(const wxDataViewItem &item) const;

	// helper methods to change the model

	void AddToClassical(const wxString &title, const wxString &artist, unsigned int year);
	
	void Delete(const wxDataViewItem &item);

	wxDataViewItem GetNinthItem() const
	{
		return wxDataViewItem(m_ninth);
	}

	// override sorting to always sort branches ascendingly

	int Compare(const wxDataViewItem &item1, const wxDataViewItem &item2,
		unsigned int column, bool ascending) const wxOVERRIDE;

	// implementation of base class virtuals to define model

	virtual unsigned int GetColumnCount() const wxOVERRIDE
	{
		return 6;
	}

	virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE
	{
		if (col == 2)
			return wxT("long");

		return wxT("string");
	}

	virtual void GetValue(wxVariant &variant,
		const wxDataViewItem &item, unsigned int col) const wxOVERRIDE;
	virtual bool SetValue(const wxVariant &variant,
		const wxDataViewItem &item, unsigned int col) wxOVERRIDE;

	virtual bool IsEnabled(const wxDataViewItem &item,
		unsigned int col) const wxOVERRIDE;

	virtual wxDataViewItem GetParent(const wxDataViewItem &item) const wxOVERRIDE;
	virtual bool IsContainer(const wxDataViewItem &item) const wxOVERRIDE;
	virtual unsigned int GetChildren(const wxDataViewItem &parent,
		wxDataViewItemArray &array) const wxOVERRIDE;

private:
	MyMusicTreeModelNode*   m_root;

	// pointers to some "special" nodes of the tree:
	MyMusicTreeModelNode*   m_pop;
	MyMusicTreeModelNode*   m_classical;
	MyMusicTreeModelNode*   m_ninth;

	// ??
	bool                    m_classicalMusicIsKnownToControl;
};
