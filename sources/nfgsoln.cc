//
// FILE: nfgsoln.cc --  all solution display related routines for the normal
// form.
//
// $Id$
//

#include "wx/wx.h"

#include "gambit.h"
#include "nfgconst.h"
#include "nfgshow.h"
#include "nfgsoln.h"
#include "nfplayer.h"

#ifdef NOT_PORTED_YET

//=========================================================================
//                      class dialogMixedSortFilter
//=========================================================================

class dialogMixedSortFilter : public guiAutoDialog {
private:
  MSolnSortFilterOptions &options;
  wxRadioBox *m_sortBy;
  wxListBox *m_filterCreator, *m_filterNash, *m_filterPerfect, *m_filterProper;

public:
  dialogMixedSortFilter(MSolnSortFilterOptions &options);
  virtual ~dialogMixedSortFilter();
};


dialogMixedSortFilter::dialogMixedSortFilter(MSolnSortFilterOptions &options_)
  : guiAutoDialog(0, "Sort & Filter"), options(options_)
{
  wxString sortByChoices[] = { "ID", "Creator", "Nash", "Perfect", 
			       "Qre Value", "Qre Lambda", "Liap Value" };
  m_sortBy = new wxRadioBox(this, -1, "Sort By",
			    wxDefaultPosition, wxDefaultSize,
			    7, sortByChoices);
  m_sortBy->SetSelection(options.SortBy() - 1);
  m_sortBy->SetConstraints(new wxLayoutConstraints);
  m_sortBy->GetConstraints()->left.SameAs(this, wxLeft, 10);
  m_sortBy->GetConstraints()->top.SameAs(this, wxTop, 10);
  m_sortBy->GetConstraints()->width.AsIs();
  m_sortBy->GetConstraints()->height.AsIs();

  wxStaticBox *filterGroup = new wxStaticBox(this, -1, "Filter By");

  m_filterCreator = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize,
				  NUM_MCREATORS, options.filter_cr_str + 1,
				  wxLB_MULTIPLE);
  int listWidth = (int) m_filterCreator->GetCharWidth() * 14;
  for (int i = 1; i <= NUM_MCREATORS; i++)
    m_filterCreator->SetSelection(i - 1, options.FilterCr()[i]);
  m_filterCreator->SetConstraints(new wxLayoutConstraints);
  m_filterCreator->GetConstraints()->top.SameAs(filterGroup, wxTop, 20);
  m_filterCreator->GetConstraints()->left.SameAs(filterGroup, wxLeft, 10);
  m_filterCreator->GetConstraints()->height.AsIs();
  m_filterCreator->GetConstraints()->width.Absolute(listWidth);

  m_filterNash = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize,
			       3, options.filter_tri_str + 1,
			       wxLB_MULTIPLE);
  m_filterNash->SetConstraints(new wxLayoutConstraints);
  m_filterNash->GetConstraints()->top.SameAs(m_filterCreator, wxTop);
  m_filterNash->GetConstraints()->left.SameAs(m_filterCreator, wxRight, 10);
  m_filterNash->GetConstraints()->height.AsIs();
  m_filterNash->GetConstraints()->width.PercentOf(m_filterCreator,
						  wxWidth, 75);

  m_filterPerfect = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize,
				  3, options.filter_tri_str + 1,
				  wxLB_MULTIPLE);
  m_filterPerfect->SetConstraints(new wxLayoutConstraints);
  m_filterPerfect->GetConstraints()->top.SameAs(m_filterNash, wxTop);
  m_filterPerfect->GetConstraints()->left.SameAs(m_filterNash, wxRight, 10);
  m_filterPerfect->GetConstraints()->height.AsIs();
  m_filterPerfect->GetConstraints()->width.PercentOf(m_filterCreator,
						     wxWidth, 75);

  m_filterProper = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize,
				 3, options.filter_tri_str + 1,
				 wxLB_MULTIPLE);
  m_filterProper->SetConstraints(new wxLayoutConstraints);
  m_filterProper->GetConstraints()->top.SameAs(m_filterPerfect, wxTop);
  m_filterProper->GetConstraints()->left.SameAs(m_filterPerfect, wxRight, 10);
  m_filterProper->GetConstraints()->height.AsIs();
  m_filterProper->GetConstraints()->width.PercentOf(m_filterCreator,
						    wxWidth, 75);

  filterGroup->SetConstraints(new wxLayoutConstraints);
  filterGroup->GetConstraints()->left.SameAs(this, wxLeft, 10);
  filterGroup->GetConstraints()->top.SameAs(m_sortBy, wxBottom, 10);
  filterGroup->GetConstraints()->right.SameAs(m_filterProper, wxRight, -10);
  filterGroup->GetConstraints()->bottom.SameAs(m_filterCreator, wxBottom, -10);

  m_okButton->GetConstraints()->top.SameAs(filterGroup, wxBottom, 10);
  m_okButton->GetConstraints()->right.SameAs(m_cancelButton, wxLeft, 10);
  m_okButton->GetConstraints()->width.SameAs(m_cancelButton, wxWidth);
  m_okButton->GetConstraints()->height.AsIs();

  m_cancelButton->GetConstraints()->centreY.SameAs(m_okButton, wxCentreY);
  m_cancelButton->GetConstraints()->centreX.SameAs(this, wxCentreX);
  m_cancelButton->GetConstraints()->width.AsIs();
  m_cancelButton->GetConstraints()->height.AsIs();

  m_helpButton->GetConstraints()->centreY.SameAs(m_okButton, wxCentreY);
  m_helpButton->GetConstraints()->left.SameAs(m_cancelButton, wxRight, 10);
  m_helpButton->GetConstraints()->width.SameAs(m_cancelButton, wxWidth);
  m_helpButton->GetConstraints()->height.AsIs();

  for (int i = 1; i <= 3; i++) {
    m_filterNash->SetSelection(i - 1, options.FilterNash()[i]);
    m_filterPerfect->SetSelection(i - 1, options.FilterPerfect()[i]);
    m_filterProper->SetSelection(i - 1, options.FilterProper()[i]);
  }

  Go();
}

dialogMixedSortFilter::~dialogMixedSortFilter()
{
  if (Completed() == wxOK) {
    options.SortBy() = m_sortBy->GetSelection() + 1;

    for (int i = 1; i <= NUM_MCREATORS; i++)
      options.FilterCr()[i] = m_filterCreator->Selected(i - 1);

    for (int i = 1; i <= 3; i++) {
      options.FilterNash()[i] = m_filterNash->Selected(i - 1);
      options.FilterPerfect()[i] = m_filterPerfect->Selected(i - 1);
      options.FilterProper()[i] = m_filterProper->Selected(i - 1);
    }
  }
}
#endif // NOT_PORTED_YET

//****************************************************************************
//                        NORM SOLUTION SHOW
//****************************************************************************
// Note: buttons should be specified in the constructor, but due to a but
// in wx_motif, they fail to appear.  So, we create them later by calling
// MakeButtons.  Fix this once wx_motif is fixed!

#define MSOLN_DYNAMIC       0       // Optional features.
#define MSOLN_ZERPROB       1
#define MSOLN_EQUVALS       2
#define MSOLN_CREATOR       3
#define MSOLN_ISNASH        4
#define MSOLN_ISPERF        5
#define MSOLN_ISPROP        6
#define MSOLN_GLAMBDA       7
#define MSOLN_GVALUE        8
#define MSOLN_LVALUE        9

#define MSOLN_NUM_FEATURES  10

#define MSOLN_ID            20      // These are always on
#define MSOLN_PLAYER        21

char *NfgSolnShow::feature_names[MSOLN_NUM_FEATURES] = 
{
    "Update Dynamically", "Zero Prob", "Equ Values",
    "Creator", "Nash", "Perfect", "Proper",
    "QRE Lambda", "QRE Value", "Liap Value"
};

int NfgSolnShow::feature_width[MSOLN_NUM_FEATURES] = 
{ 0, 0, -1, 8, 2, 2, 2, 7, 7, 7};



NfgSolnShow::NfgSolnShow(gSortList<MixedSolution> &solns_, int num_players_,
                         int max_strats, int cur_soln_, NormalDrawSettings &ds,
                         MSolnSortFilterOptions &sf_options_, NfgShow *parent_)
  : wxGrid(parent_, -1, wxDefaultPosition, wxDefaultSize),
    parent(parent_), solns(solns_), num_players(num_players_),
    cur_soln(cur_soln_), num_solutions(solns_.Length()),
    norm_draw_settings(ds), features(0, MSOLN_NUM_FEATURES - 1),
    sf_options(sf_options_)
{
  CreateGrid(solns.Length() * num_players, 4);
  SetEditable(false);
  SetLabelSize(wxVERTICAL, 0);

  for (int i = 1; i < MSOLN_NUM_FEATURES; i++) {
    features[i] = 1;

    int col = FeaturePos(i);
    InsertCols(col, 1, false);
    SetLabelValue(wxHORIZONTAL, (char *) feature_names[i], col);
  }

  SetLabelValue(wxHORIZONTAL, "ID", FeaturePos(MSOLN_ID));
  SetLabelValue(wxHORIZONTAL, "Player", FeaturePos(MSOLN_PLAYER));

  for (int i = FeaturePos(MSOLN_NUM_FEATURES-1)+1; i < GetCols(); i++) {
    SetLabelValue(wxHORIZONTAL, "", i);
  }

  if (cur_soln) {
    SetGridCursor(SolnPos(cur_soln) - 1, 0);
    OnSelectCell(SolnPos(cur_soln) - 1, 0);
  }

  SortFilter(false);
  AdjustScrollbars();
  UpdateValues();
}

int NfgSolnShow::SolnNum(int row)
{
  return (row / num_players + 1);
}

int NfgSolnShow::SolnPos(int soln_num)
{
  return (soln_num - 1) * num_players;
}


// Update Soln
void NfgSolnShow::UpdateSoln(int row, int col)
{
  // passing -1 for the col indicates the row is the new solution #
  int new_soln;

  if (col != -1) 
    new_soln = SolnNum(row);
  else 
    new_soln = row;

  if (new_soln != cur_soln) {
    // remove the old hilight, hilight the new cell
    cur_soln = new_soln;

    // figure out which solution was clicked on, and inform the parent
    if (parent) 
      parent->ChangeSolution(cur_soln);
  }
}


void NfgSolnShow::OnDoubleClick(int row, int col, int , const gText &)
{
#ifdef NOT_PORTED_YET
    if (col == FeaturePos(MSOLN_ID)) 
        UpdateSoln(row, col);   // change solution

    if (col > FeaturePos(MSOLN_NUM_FEATURES - 1))   // edit solution
    {
        SetCurRow(row);
        SetCurCol(col);
        OnEdit();
    }

    // Double clicking on the first row in any optional feature col, sorts by that col
    if (row == 1)
    {
        int old_sort_by = sf_options.SortBy();

        if (col == FeaturePos(MSOLN_CREATOR) && features[MSOLN_CREATOR])
            sf_options.SortBy() = MSORT_BY_CREATOR;

        if (col == FeaturePos(MSOLN_ISNASH) && features[MSOLN_ISNASH])
            sf_options.SortBy() = MSORT_BY_NASH;

        if (col == FeaturePos(MSOLN_ISPERF) && features[MSOLN_ISPERF])
            sf_options.SortBy() = MSORT_BY_PERFECT;

        if (col == FeaturePos(MSOLN_ISPROP) && features[MSOLN_ISPROP])
            sf_options.SortBy() = MSORT_BY_PROPER;

        if (col == FeaturePos(MSOLN_GLAMBDA) && features[MSOLN_GLAMBDA])
            sf_options.SortBy() = MSORT_BY_GLAMBDA;

        if (col == FeaturePos(MSOLN_GVALUE) && features[MSOLN_GVALUE])
            sf_options.SortBy() = MSORT_BY_GVALUE;

        if (col == FeaturePos(MSOLN_LVALUE) && features[MSOLN_LVALUE])
            sf_options.SortBy() = MSORT_BY_LVALUE;

        if (old_sort_by != sf_options.SortBy()) SortFilter(false);
    }
#endif  // NOT_PORTED_YET
}

void NfgSolnShow::OnSelectCell(int p_row, int p_col)
{
  for (int i = (cur_soln - 1) * parent->Game().NumPlayers();
       i < cur_soln * parent->Game().NumPlayers(); i++) {
    for (int j = 0; j < GetCols(); j++) {
      SetCellBackgroundColour(*wxWHITE, i, j);
    }
  }

  cur_soln = SolnNum(p_row);

  for (int i = (cur_soln - 1) * parent->Game().NumPlayers();
       i < cur_soln * parent->Game().NumPlayers(); i++) {
    for (int j = 0; j < GetCols(); j++) {
      SetCellBackgroundColour(*wxCYAN, i, j);
    }
  }

  if (parent) {
    parent->ChangeSolution(cur_soln);
  }
}

int NfgSolnShow::FeaturePos(int feature)
{
  switch(feature) {
    // These features are always on and always in the same columns
  case MSOLN_ID: 
    return 0;
  case MSOLN_PLAYER: 
    return 1;

    // The rest of the features are optional and may change location
  default:  {
      int pos = 2;

      for (int i = 0; i <= feature; i++) {
	if (features[i] && feature_width[i] > 0) 
	  pos++;
      }

      return pos;
    }
  }
}

#ifdef NOT_PORTED_YET

class dialogMixedSettings : public guiAutoDialog {
private:
  wxCheckBox *m_dynamic, *m_zeroProb;
  wxCheckBox *m_values, *m_creator, *m_nash, *m_perfect;
  wxCheckBox *m_qreLambda, *m_qreValue, *m_liapValue;

public:
  dialogMixedSettings(wxWindow *, const gArray<bool> &);
  virtual ~dialogMixedSettings() { }

  bool UpdateDynamically(void) const { return m_dynamic->GetValue(); }
  bool ShowZeroProbs(void) const { return m_zeroProb->GetValue(); }

  bool ShowValues(void) const { return m_values->GetValue(); }
  bool ShowCreator(void) const { return m_creator->GetValue(); }
  bool ShowNash(void) const { return m_nash->GetValue(); }
  bool ShowPerfect(void) const { return m_perfect->GetValue(); }
  bool ShowQreLambda(void) const { return m_qreLambda->GetValue(); }
  bool ShowQreValue(void) const { return m_qreValue->GetValue(); }
  bool ShowLiapValue(void) const { return m_liapValue->GetValue(); }
};

dialogMixedSettings::dialogMixedSettings(wxWindow *p_parent,
					 const gArray<bool> &p_options)
  : guiAutoDialog(p_parent, "Display options")
{
  m_dynamic = new wxCheckBox(this, -1, "Update dynamically");
  m_dynamic->SetValue(p_options[0]);
  m_dynamic->SetConstraints(new wxLayoutConstraints);
  m_dynamic->GetConstraints()->top.SameAs(this, wxTop, 10);
  m_dynamic->GetConstraints()->left.SameAs(this, wxLeft, 10);
  m_dynamic->GetConstraints()->width.AsIs();
  m_dynamic->GetConstraints()->height.AsIs();

  m_zeroProb = new wxCheckBox(this, -1, "Show zero probability strategies");
  m_zeroProb->SetValue(p_options[1]);
  m_zeroProb->SetConstraints(new wxLayoutConstraints);
  m_zeroProb->GetConstraints()->top.SameAs(m_dynamic, wxBottom, 10);
  m_zeroProb->GetConstraints()->left.SameAs(m_dynamic, wxLeft);
  m_zeroProb->GetConstraints()->width.AsIs();
  m_zeroProb->GetConstraints()->height.AsIs();

  wxStaticBox *propertyGroup = new wxStaticBox(this, -1, "Show properties");

  m_creator = new wxCheckBox(this, -1, "Creator");
  m_creator->SetValue(p_options[3]);
  m_creator->SetConstraints(new wxLayoutConstraints);
  m_creator->GetConstraints()->top.SameAs(propertyGroup, wxTop, 20);
  m_creator->GetConstraints()->left.SameAs(propertyGroup, wxLeft, 10);
  m_creator->GetConstraints()->width.AsIs();
  m_creator->GetConstraints()->height.AsIs();

  m_nash = new wxCheckBox(this, -1, "Nash");
  m_nash->SetValue(p_options[4]);
  m_nash->SetConstraints(new wxLayoutConstraints);
  m_nash->GetConstraints()->top.SameAs(m_creator, wxBottom, 10);
  m_nash->GetConstraints()->left.SameAs(m_creator, wxLeft);
  m_nash->GetConstraints()->width.AsIs();
  m_nash->GetConstraints()->height.AsIs();

  m_perfect = new wxCheckBox(this, -1, "Perfect");
  m_perfect->SetValue(p_options[5]);
  m_perfect->SetConstraints(new wxLayoutConstraints);
  m_perfect->GetConstraints()->top.SameAs(m_nash, wxBottom, 10);
  m_perfect->GetConstraints()->left.SameAs(m_nash, wxLeft);
  m_perfect->GetConstraints()->width.AsIs();
  m_perfect->GetConstraints()->height.AsIs();

  m_values = new wxCheckBox(this, -1, "Equilibrium values");
  m_values->SetValue(p_options[2]);
  m_values->SetConstraints(new wxLayoutConstraints);
  m_values->GetConstraints()->top.SameAs(m_creator, wxTop);
  m_values->GetConstraints()->left.SameAs(m_perfect, wxRight, 10);
  m_values->GetConstraints()->width.AsIs();
  m_values->GetConstraints()->height.AsIs();

  m_qreValue = new wxCheckBox(this, -1, "Qre value");
  m_qreValue->SetValue(p_options[8]);
  m_qreValue->SetConstraints(new wxLayoutConstraints);
  m_qreValue->GetConstraints()->top.SameAs(m_values, wxBottom, 10);
  m_qreValue->GetConstraints()->left.SameAs(m_values, wxLeft);
  m_qreValue->GetConstraints()->width.AsIs();
  m_qreValue->GetConstraints()->height.AsIs();
					   
  m_qreLambda = new wxCheckBox(this, -1, "Qre lambda");
  m_qreLambda->SetValue(p_options[7]);
  m_qreLambda->SetConstraints(new wxLayoutConstraints);
  m_qreLambda->GetConstraints()->top.SameAs(m_qreValue, wxBottom, 10);
  m_qreLambda->GetConstraints()->left.SameAs(m_qreValue, wxLeft);
  m_qreLambda->GetConstraints()->width.AsIs();
  m_qreLambda->GetConstraints()->height.AsIs();

  m_liapValue = new wxCheckBox(this, -1, "Liap value");
  m_liapValue->SetValue(p_options[9]);
  m_liapValue->SetConstraints(new wxLayoutConstraints);
  m_liapValue->GetConstraints()->top.SameAs(m_qreLambda, wxBottom, 10);
  m_liapValue->GetConstraints()->left.SameAs(m_qreLambda, wxLeft);
  m_liapValue->GetConstraints()->height.AsIs();
  m_liapValue->GetConstraints()->width.AsIs();

  propertyGroup->SetConstraints(new wxLayoutConstraints);
  propertyGroup->GetConstraints()->top.SameAs(m_zeroProb, wxBottom, 10);
  propertyGroup->GetConstraints()->left.SameAs(m_zeroProb, wxLeft);
  propertyGroup->GetConstraints()->bottom.SameAs(m_liapValue, wxBottom, -10);
  propertyGroup->GetConstraints()->right.SameAs(m_values, wxRight, -10);

  m_okButton->GetConstraints()->top.SameAs(propertyGroup, wxBottom, 10);
  m_okButton->GetConstraints()->right.SameAs(m_cancelButton, wxLeft, 10);
  m_okButton->GetConstraints()->width.SameAs(m_cancelButton, wxWidth);
  m_okButton->GetConstraints()->height.AsIs();

  m_cancelButton->GetConstraints()->centreY.SameAs(m_okButton, wxCentreY);
  m_cancelButton->GetConstraints()->centreX.SameAs(this, wxCentreX);
  m_cancelButton->GetConstraints()->width.AsIs();
  m_cancelButton->GetConstraints()->height.AsIs();

  m_helpButton->GetConstraints()->centreY.SameAs(m_okButton, wxCentreY);
  m_helpButton->GetConstraints()->left.SameAs(m_cancelButton, wxRight, 10);
  m_helpButton->GetConstraints()->width.SameAs(m_cancelButton, wxWidth);
  m_helpButton->GetConstraints()->height.AsIs();

  Go();
}

#endif  // NOT_PORTED_YET

void NfgSolnShow::SetOptions(void)
{
#ifdef NOT_PORTED_YET
  dialogMixedSettings dialog(this, features);

  gArray<bool> newFeatures = features;

  if (dialog.Completed() == wxOK) {
    newFeatures[0] = dialog.UpdateDynamically();
    newFeatures[1] = dialog.ShowZeroProbs();
    newFeatures[2] = dialog.ShowValues();
    newFeatures[3] = dialog.ShowCreator();
    newFeatures[4] = dialog.ShowNash();
    newFeatures[5] = dialog.ShowPerfect();
    newFeatures[7] = dialog.ShowQreLambda();
    newFeatures[8] = dialog.ShowQreValue();
    newFeatures[9] = dialog.ShowLiapValue();

    for (int i = MSOLN_NUM_FEATURES - 1; i >= 0; i--) {
      if (features[i] && !newFeatures[i]) {
	if (feature_width[i]) 
	  DelCol(FeaturePos(i));

	features[i] = 0;
      }
    }

    for (int i = 0; i < MSOLN_NUM_FEATURES; i++) {
      if (newFeatures[i] && !features[i]) {
	if (feature_width[i]) {
	  int col = FeaturePos(i) + 1;
	  AddCol(col);
	  SetCell(1, col, feature_names[i]);
	  Bold(1, col, 0, TRUE);

	  if (feature_width[i] == -1)  // precision dependent
	    DrawSettings()->SetColWidth(2 + parent->GetDecimals(), col);
	  else                         // precision independent
	    DrawSettings()->SetColWidth(feature_width[i], col);
	}
	
	features[i] = 1;
      }
    }

    UpdateValues();
    Repaint();
  }
#endif  // NOT_PORTED_YET
}


void NfgSolnShow::UpdateValues(void)
{
  int sp = FeaturePos(MSOLN_NUM_FEATURES - 1);    // first column with actual strategies

  for (int i = 1; i <= num_solutions; i++) {
    int cur_pos = (i - 1)*num_players;
    NFSupport support(solns[i].Game());
    SetCellValue((char *) ToText((int) solns[i].Id()),
		 cur_pos, FeaturePos(MSOLN_ID));

    SetCellValue((char *) ToText(solns[i].Creator()),
		 cur_pos, FeaturePos(MSOLN_CREATOR));

    SetCellValue((char *) ToText(solns[i].IsNash()),
		 cur_pos, FeaturePos(MSOLN_ISNASH));

    SetCellValue((char *) ToText(solns[i].IsPerfect()),
		 cur_pos, FeaturePos(MSOLN_ISPERF));

    SetCellValue((char *) ToText(solns[i].IsProper()),
		 cur_pos, FeaturePos(MSOLN_ISPROP));

    if (solns[i].Creator() == algorithmNfg_QRE) {
      SetCellValue((char *) ToText(solns[i].QreLambda(),parent->GetDecimals()),
		   cur_pos, FeaturePos(MSOLN_GLAMBDA));
    }
    else {
      SetCellValue("--------", cur_pos, FeaturePos(MSOLN_GLAMBDA));
    }

    if (solns[i].Creator() == algorithmNfg_QRE) {
      SetCellValue((char *) ToText(solns[i].QreValue(), parent->GetDecimals()),
		   cur_pos, FeaturePos(MSOLN_GVALUE));
    }
    else {
      SetCellValue("--------", cur_pos, FeaturePos(MSOLN_GVALUE));
    }

    SetCellValue((char *) ToText(solns[i].LiapValue(), parent->GetDecimals()),
		 cur_pos, FeaturePos(MSOLN_LVALUE));

    for (int j = 1; j <= num_players; j++) {
      SetCellValue((char *) ToText(solns[i].Payoff(j), parent->GetDecimals()),
		   cur_pos + (j - 1), FeaturePos(MSOLN_EQUVALS)); 
    }

    for (int j = 1; j <= num_players; j++) {
      gText pl_name = support.Game().Players()[j]->GetName();

      if (pl_name == "") 
	pl_name = ToText(j);

      SetCellValue((char *) pl_name,
		   cur_pos + (j - 1), FeaturePos(MSOLN_PLAYER));

      int k1 = 1;

      for (int k = 1; k <= solns[i].Lengths()[j]; k++) {
	Strategy *strategy = support.Game().Players()[j]->Strategies()[k];
	gText tmp_str = "";

	if (support.Strategies(j)[k]->Name() != "")
	  tmp_str += support.Strategies(j)[k]->Name();
	else
	  tmp_str += ToText(k);

	tmp_str += ": " + ToText(solns[i](strategy));
	SetCellValue((char *) tmp_str, cur_pos + (j - 1), sp + k1);
	k1++;
      }

      for (; k1 <= solns[i].Lengths()[j]; k1++) {
	SetCellValue("", cur_pos + (j - 1), sp + k1);
      }
    }
  }
}


void NfgSolnShow::SolutionToExtensive(void)
{
#ifdef NOT_PORTED_YET
    assert(parent);     // we must have a parent if we got here

    // must have somewhere to send solutions
    if (!parent->InterfaceOk())  return;

    int row = CurRow();

    if (row == 1) 
        return;   // first row = no solution selected

    int new_soln = (row - 1) / num_players + 1;

    if ((row - 1) % num_players == 0) 
        new_soln -= 1;

    if (new_soln < 1) 
        new_soln = 1;

    if (new_soln > solns.Length()) 
        new_soln = solns.Length();

    parent->SolutionToExtensive(solns[new_soln], true);
#endif  // NOT_PORTED_YET
}

void NfgSolnShow::SortFilter(bool inter)
{
#ifdef NOT_PORTED_YET
  int completed = wxOK;

  if (inter)  { 
    // interactive or automatic
    completed = dialogMixedSortFilter(sf_options).Completed();
  }

    if (completed == wxOK)
    {
        int old_num_sol = num_solutions;  // current state
        const MixedSolution *cur_solnp = 0;

        if (cur_soln) 
            cur_solnp = &solns[cur_soln];

        MSolnSorterFilter SF(solns, sf_options);
        int i, j;
        int new_soln = 0;   // try to find the new pos of cur_soln

        for (i = 1; i <= solns.VisibleLength(); i++) 
        {
            if (cur_solnp == &solns[i]) 
                new_soln = i;
        }

        int new_num_sol = solns.VisibleLength();

        if (old_num_sol > new_num_sol)
        {
            for (i = old_num_sol; i > new_num_sol; i--)
                for (j = 1; j <= num_players; j++)
                    DelRow();
        }

        if (old_num_sol < new_num_sol)
        {
            for (i = old_num_sol + 1; i <= new_num_sol; i++)
                for (j = 1; j <= num_players; j++)
                    AddRow();
        }

        num_solutions = solns.VisibleLength();
        UpdateValues();

        // make sure we do not try to access non-displayed solutions
        if (cur_soln > solns.VisibleLength()) 
            cur_soln = 0;

        UpdateSoln(new_soln, -1);
        SetCurRow(SolnPos(new_soln));
        SetCurCol(1);

        if (old_num_sol != new_num_sol) 
            Redraw();

        Repaint();
    }
#endif  // NOT_PORTED_YET
}

void NfgSolnShow::OnRemove(bool all)
{
#ifdef NOT_PORTED_YET
    if (solns.Length() == 0)
    {
        wxMessageBox("Solution list is empty");
        return;
    }

    if (!all)
    {
        int row = CurRow();

        if (row == 1)
        {
            wxMessageBox("Remove what?");
            return;
        }

        int soln_num = SolnNum(row);

        solns.Remove(soln_num);

        if (soln_num == cur_soln) 
            UpdateSoln(1, 1);  // unselect all solutions

        for (int i = num_players; i >= 1; i--) 
            DelRow(2 + (soln_num - 1)*num_players + (i - 1));

        num_solutions--;
    }
    else
    {
        UpdateSoln(1, 1);    // unselect all solutions

        while (solns.Length()) 
            solns.Remove(1);    // del all solutions

        while (GetRows() > 1) 
            DelRow(2);  // del all rows after row 1

        num_solutions = 0;
    }

    CanvasFocus();
    UpdateValues();
    Redraw();
#endif  // NOT_PORTED_YET
}

#ifdef NOT_PORTED_YET
class MixedSolnEdit : public SpreadSheet3D {
private:
  MixedSolution &soln;
  gArray<int> dim;

public:
  MixedSolnEdit(MixedSolution &soln_, wxFrame *parent, int decimals);
  void OnSelectedMoved(int row, int col, SpreadMoveDir /*how*/);
  void OnOk(void);
  bool OnClose(void);
};

MixedSolnEdit::MixedSolnEdit(MixedSolution &soln_, wxFrame *parent,
			     int decimals)
  :  SpreadSheet3D(soln_.Game().NumPlayers() + 1,
		   gmax(NFSupport(soln_.Game()).NumStrats()) + 1,
		   1, 2, "Edit Mixed Solution", parent, ANY_BUTTON),
     soln(soln_)
{
  Show(FALSE);
  int i, j;
  NFSupport sup(soln.Game());
  dim = sup.NumStrats();
  int max_dim = gmax(dim);
  int num_players = dim.Length();

  DrawSettings()->SetColWidth(8, 1);

  for (i = 1; i <= num_players; i++) {   // label rows
    gText tmp_str = sup.Game().Players()[i]->GetName();

    if (tmp_str == "") 
      tmp_str = "Player " + ToText(i);

    SetCell(i + 1, 1, tmp_str);
    Bold(i + 1, 1, 0, TRUE);
  }
  
  for (i = 1; i <= max_dim; i++) {  // label cols
    SetCell(1, i + 1, ToText(i));
    Bold(1, i + 1, 0, TRUE);
    DrawSettings()->SetColWidth(2 + decimals, i + 1);
  }

  for (i = 1; i <= num_players; i++)  {
    // enter values
    for (j = 1; j <= dim[i]; j++) {
      Strategy *strategy = soln.Game().Players()[i]->Strategies()[j];
      SetCell(i + 1, j + 1, ToText(soln(strategy), decimals));
      SetType(i + 1, j + 1, gSpreadStr);
    }
    
    for (j = dim[i] + 1; j <= max_dim; j++) 
      HiLighted(i + 1, j + 1, 1, TRUE);
  }

  SetCurCol(2);
  SetCurRow(2);
  MakeButtons(OK_BUTTON|CANCEL_BUTTON|PRINT_BUTTON|HELP_BUTTON);
  Redraw();
  Show(TRUE);
}

bool MixedSolnEdit::OnClose(void)
{
  SetCompleted(wxCANCEL);
  Show(FALSE);
  return FALSE;
}

void MixedSolnEdit::OnSelectedMoved(int row, int col, SpreadMoveDir /*how*/)
{
  if (row == 1) {
    SetCurRow(2);
    row = 2;
  }

  if (col - 1 > dim[row-1]) 
    SetCurCol(dim[row-1] + 1);

  if (col == 1) 
    SetCurCol(2);
}

// OnOK
void MixedSolnEdit::OnOk(void)
{
  for (int i = 1; i <= dim.Length(); i++)
    for (int j = 1; j <= dim[i]; j++) {
      gNumber value;
      FromText(GetCell(i + 1, j + 1), value);
      soln.Set(soln.Game().Players()[i]->Strategies()[j], value);
    }
  SetCompleted(wxOK);
  Show(FALSE);
}
#endif  // NOT_PORTED_YET

// OnAdd
void NfgSolnShow::OnAdd(void)
{
#ifdef NOT_PORTED_YET
    MixedSolution temp_soln(parent->CreateSolution());
    MixedSolnEdit *add_dialog = new MixedSolnEdit(temp_soln, this,
						  parent->GetDecimals());
    Enable(FALSE);  // disable this window until the edit window is close

    while (add_dialog->Completed() == wxRUNNING) 
        wxYield();

    Enable(TRUE);
    CanvasFocus();

    if (add_dialog->Completed() == wxOK)
    {
        solns.Append(temp_soln);
        SortFilter(false);
    }

    delete add_dialog;
#endif  // NOT_PORTED_YET
}

// OnEdit
void NfgSolnShow::OnEdit(void)
{
#ifdef NOT_PORTED_YET
    if (solns.Length() == 0)
    {
        wxMessageBox("Solution list is empty");
        return;
    }

    int row = CurRow();

    if (row == 1)
    {
        wxMessageBox("Edit what?");
        return;
    }

    int soln_num = SolnNum(row);

    MixedSolution temp_soln = solns[soln_num];
    MixedSolnEdit *add_dialog = new MixedSolnEdit(temp_soln, this,
						  parent->GetDecimals());
    Enable(FALSE);  // disable this window until the edit window is close

    while (add_dialog->Completed() == wxRUNNING) 
        wxYield();

    Enable(TRUE);
    CanvasFocus();

    if (add_dialog->Completed() == wxOK)
    {
        solns[soln_num] = temp_soln;
        SortFilter(false);

        if (cur_soln == soln_num) 
            parent->ChangeSolution(cur_soln);
    }

    delete add_dialog;
#endif  // NOT_PORTED_YET
}


#ifdef NOT_PORTED_YET


//****************************************************************************
//                       MIXED SOLUTION PICKER (single)
//****************************************************************************

Nfg1SolnPicker::Nfg1SolnPicker(gSortList<MixedSolution > &soln, int num_players, int max_strats,
                               int cur_soln_, NormalDrawSettings    &ds,
                               MSolnSortFilterOptions &sf_options,
                               NfgShow *parent_, wxFrame *parent_frame)
    : NfgSolnShow(soln, num_players, max_strats, cur_soln_, ds, 
                  sf_options, parent_, parent_frame, MSOLN_O_EDIT)
{
    SetTitle("Pick a profile to start with");
    picked = 0;
}


void Nfg1SolnPicker::PickSoln(int row)
{
    // figure out which solution was clicked on
    int new_soln = SolnNum(row);

    if (new_soln != picked)
    {
        if (picked) 
            HiLighted(SolnPos(picked), 1, 0, FALSE);   // un-highlight this solution

        if (new_soln) 
            HiLighted(SolnPos(new_soln), 1, 0, TRUE);  // highlight this solution

        picked = new_soln;
    }

    Repaint();
}


void Nfg1SolnPicker::OnDoubleClick(int row, int col, int , const gText &)
{
    if (col > FeaturePos(MSOLN_NUM_FEATURES - 1))   // edit solution
    {
        SetCurRow(row);
        SetCurCol(col);
        OnEdit();
    }

    if (col == 1) 
        PickSoln(row);
}


void Nfg1SolnPicker::OnSelectedMoved(int , int , SpreadMoveDir )
{ }


// Override help system

void Nfg1SolnPicker::OnHelp(int help_type)
{
    if (!help_type) // contents
        wxHelpContents(NFG_SOLNSTART_HELP);
    else
        wxHelpAbout();
}


void Nfg1SolnPicker::OnOk(void)
{
    if (!picked && solns.Length() != 0)
        wxMessageBox("You must pick a starting point");
    else
        SetCompleted(wxOK);
}


// OnRemove

void Nfg1SolnPicker::OnRemove(bool all)
{
    if (solns.Length() == 0)
    {
        wxMessageBox("Solution list is empty");
        return;
    }

    if (!all)
    {
        int row = CurRow();

        if (row == 1)
        {
            wxMessageBox("Remove what?");
            return;
        }

        int soln_num = SolnNum(row);

        if (soln_num == picked) 
            picked = 0;
    }
    else
    {
        picked = 0;
    }

    NfgSolnShow::OnRemove(all);
}


int Nfg1SolnPicker::Picked(void) const
{
    return picked;
}

// OnClose -- Close the window, as if OK was pressed
bool Nfg1SolnPicker::OnClose(void)
{
  OnOk();
  return FALSE;
}

#endif  // NOT_PORTED_YET



