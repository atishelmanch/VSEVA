
#ifndef vseva_hh
#define vseva_hh 1

/*
root -l -b -q make_channels.C
root -l -b -q split.C
root -l -b -q tmva_train.C
*/

namespace vseva {

  class DataSet {
    public:
    DataSet(){}

    DataSet(string process_name, string latex_name, string short_name, string type){
      this->process_name = process_name;
      this->latex_name   = latex_name;
      this->short_name   = short_name;
      this->type         = type;
      nevents = 0;
      weight_sum = 0;
      sys = false;
    }

    string process_name, latex_name, short_name, type, dataset_ttree_name;
    vector<string> files;
    vector<string> sys_dataset_ttree_name;
    long long int nevents;
    double weight_sum, xsec;
    bool sys;

    void AddFile(string path){
      files.push_back( path );
    }

    bool IsData(){ return (type == "D" or type == "Data"); }
    bool IsSignal(){ return (type == "S" or type == "Signal"); }
    bool IsBackground(){ return (type == "B" or type == "Background"); }

    vector<TH1D*> hists;
  };

  class PlotVariable{
    public:
    PlotVariable(){}
    /*PlotVariable(string key, double min, double max){
      this->key = key;
      this->min = min;
      this->max = max;
      this->label = "";
      this->draw_type = "";
    }

    PlotVariable(string key, string lab, int nbins, double min, double max, string process_name = "", string expression="", string weight_expression=""){
      this->key = key;
      this->min = min;
      this->max = max;
      this->label = lab;
      this->draw_type = "";
      this->process_name   = process_name;
      this->expression = expression;
      this->weight_expression = weight_expression;
    }*/

    PlotVariable(string expression, string lab, int nbins, double min, double max){
      this->min = min;
      this->max = max;
      this->nbins = nbins;
      this->label = lab;
      this->expression = expression;
    }

    bool CheckProcess(std::string name){
      cout << processes_include.size() << " " << processes_exclude.size() << endl;
      if( processes_include.size() ){
        auto it = find( processes_include.begin(), processes_include.end(), name );
        if( it != processes_include.end() ) return true;
        return false;
      }
      if( processes_exclude.size() ){
        auto it = find( processes_exclude.begin(), processes_exclude.end(), name );
        if( it != processes_exclude.end() ) return false;
      }
      return true;
    }

    std::string GetOutputName(){
      if(output_name.size() ) return output_name;
      return key;
    }

    string key, label, draw_type, output_name, process_name, expression, weight_expression;
    TTreeFormula *value_f, *weight_f;
    double min, max;
    int nbins;
    vector<string> processes_include, processes_exclude;
  };

  class CustomPlotVariables{
    public:
    std::vector<PlotVariable> variables;
    std::string weight;
    std::string draw_option, label, output_name;
  };



  // STYLES & COLORS ======================================================================================================================================================================
  inline Color_t FindFreeCustomColorIndex(Color_t start = 1000){
    for (Color_t i = std::max(start, Color_t(1000)), e = 10000; i < e; ++i)
      if (!gROOT->GetColor(i)) return i;
    return -1;
  }

  class FakeTColor : public TColor{
    public : 
      FakeTColor(double r, double g, double b) : TColor(FindFreeCustomColorIndex(), r/255., g/255., b/255.){}
      int GetIndex(){ return this->fNumber; }
  };

  vector <int> global_nice_colors;
  std::vector <int> getNiceColors(int values = 25){
    if( not global_nice_colors.size() ){
      std::vector <FakeTColor*> colors = { new FakeTColor( 75., 122., 192. ),
                                           new FakeTColor( 241., 72., 75. ),
                                           new FakeTColor( 31., 140., 61.  ),
                                           new FakeTColor( 241., 121., 41. ),
                                           new FakeTColor( 170., 143., 104. ),
                                           new FakeTColor( 180., 212., 79. ),
                                           new FakeTColor( 169., 169., 169. ),
                                           new FakeTColor( 233., 90.,  154.  ),
                                           new FakeTColor( 34.,  148., 232. ),
                                           new FakeTColor( 253., 205.,  74. ),
                                           new FakeTColor( 81., 179., 157. ),
                                           new FakeTColor( 107., 76., 154. ),
                                           new FakeTColor( 255., 15., 15. ),
                                           new FakeTColor( 15., 255., 15. ),
                                           new FakeTColor( 15., 15., 255. ),
                                           new FakeTColor( 255.,  15., 255. ),
                                           new FakeTColor( 255., 255., 15.  ),
                                           new FakeTColor( 15.,  255., 255. ),
                                           new FakeTColor( 242., 203., 110. ),
                                           new FakeTColor( 220., 119., 31.  ),
                                           new FakeTColor( 148., 58.,  48.  ),
                                           new FakeTColor( 174., 124., 145. ),
                                           new FakeTColor( 105., 175., 132. ),
                                           new FakeTColor( 137., 148., 242. ),
                                           new FakeTColor( 56.,  114., 221. ),
                                           new FakeTColor( 31.,  147., 138. ),
                                           new FakeTColor( 124., 32.,  148. ),
                                           new FakeTColor( 125., 105., 174. ),
                                           new FakeTColor( 222., 56.,  135. ),
                                           new FakeTColor( 174., 174., 174. ),
                                           new FakeTColor( 125., 147.,  77. ),
                                           new FakeTColor( 211., 242., 220. ),
                                           new FakeTColor( 123., 220., 127. ) };
      for(auto color : colors)
        global_nice_colors.push_back( color->GetIndex() );
    }

    std::vector <int> colors_ids = global_nice_colors;
    values -= colors_ids.size();
    for(int i = 0; i < values; i++){
      colors_ids.push_back( i+1 );
    }
    return colors_ids;
  }

  void PrintNiceColors(){
    std::vector <int> colors = getNiceColors();
    for(auto color : colors) std::cout << color << std::endl;
  }

  void set_style_FCC(){
    int font = 132;
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);

    gStyle->SetFrameFillColor(0);
    gStyle->SetFrameFillStyle(0);

    gStyle->SetPadColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetTitleColor(1);
    gStyle->SetStatColor(0);

    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendFillColor(0);
    gStyle->SetLegendFont(font);
    
    gStyle->SetOptStat(0000000);
    gStyle->SetTextFont(font);
    gStyle->SetTextSize(0.05);
    gStyle->SetLabelFont(font,"XYZ");
    gStyle->SetTitleFont(font,"XYZ");
    gStyle->SetLabelSize(0.05,"XYZ"); //0.035
    gStyle->SetTitleSize(0.05,"XYZ");
    
    gStyle->SetTitleOffset(1.25,"X");
    gStyle->SetTitleOffset(2.05,"Y");
    gStyle->SetLabelOffset(0.02,"XY");
    
    // use bold lines and markers
    gStyle->SetMarkerStyle(8);
    gStyle->SetHistLineWidth(3);
    gStyle->SetLineWidth(1);

    gStyle->SetNdivisions(510,"xy");

    // do not display any of the standard histogram decorations
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0); //("m")
    gStyle->SetOptFit(0);
    
    //gStyle->SetPalette(1,0)
    gStyle->cd();
    gROOT->ForceStyle();
  }

  // HIST DRAWING ======================================================================================================================================================================
  template<class TMP_hist_type = TH1D> class HistDrawer {
    public:
      HistDrawer(){
        font = 132;
        logY = false;
        label_x = "X";
        label_y = "Events";
        xmin = 0; xmax = 0; 
        ymin = 0; ymax = 0;
        signal_scale = 1;
        corr_draw_option = "SCAT";
      }

      vector<TMP_hist_type*> signals, backgrounds, datas;
      int font;
      bool logY;
      string label_x, label_y, corr_draw_option;
      double xmin, xmax, ymin, ymax;
      double signal_scale;

      void Print(){
        cout << "mRoot::HistDrawer.Print() " << endl;
        cout << "signals = " << signals.size() << endl;
        for(auto hist : signals) hist->Print();
        cout << " backgroudns = " << backgrounds.size() << endl;
        for(auto hist : backgrounds) hist->Print();
        cout << " datas = " << datas.size() << endl;
        for(auto hist : datas) hist->Print();
      }

      bool IsEmpty(){
        return ((signals.size() + backgrounds.size() + datas.size()) == 0);
      }

      void SetStyleHist(TH1 * hist){
        hist->GetYaxis()->SetTitle( label_y.c_str() );
        hist->GetXaxis()->SetTitle( label_x.c_str() );

        hist->GetYaxis()->CenterTitle();
        hist->GetYaxis()->SetNdivisions(510);

        hist->GetYaxis()->SetLabelFont(132);
        hist->GetYaxis()->SetLabelOffset(0.02);
        hist->GetYaxis()->SetLabelSize(0.04);
        hist->GetYaxis()->SetTitleFont(132);
        hist->GetYaxis()->SetTitleOffset(1.5);
        hist->GetYaxis()->SetTitleSize(0.045);

        hist->GetXaxis()->SetLabelFont(132);
        hist->GetXaxis()->SetLabelOffset(0.02);
        hist->GetXaxis()->SetLabelSize(0.04);
        hist->GetXaxis()->SetTitleFont(132);
        hist->GetXaxis()->SetTitleOffset(1.5);
        hist->GetXaxis()->SetTitleSize(0.045);
        hist->SetStats(false);
      }

      void SetStyle(){
        std::vector <int> colors = getNiceColors();
        int color = 0;
        int signal_color = 2;

        for(auto hist : signals){
          // hist->SetLineColor( signal_color++ );
          hist->SetLineColor( colors.at(color++) );
          // hist->SetLineWidth( 5 );
          // hist->SetLineStyle( 7 );

          hist->SetLineWidth( 2 );
          SetStyleHist(hist);

          if(xmin != xmax) hist->GetXaxis()->SetRangeUser(xmin, xmax);
          else             hist->GetXaxis()->UnZoom();
        }

        for(auto hist : backgrounds){
          hist->SetLineColor( colors.at(color++) );
          hist->SetLineWidth( 3 );
          SetStyleHist(hist);

          if(xmin != xmax) hist->GetXaxis()->SetRangeUser(xmin, xmax);
          else             hist->GetXaxis()->UnZoom();
        }

        int datastyle_index = 20;
        for(auto hist : datas){
          SetStyleHist(hist);
          hist->SetMarkerStyle( datastyle_index++ );

          if(xmin != xmax) hist->GetXaxis()->SetRangeUser(xmin, xmax);
          else             hist->GetXaxis()->UnZoom();
        }
      }

      void SetMinMax(double minh_v, double maxh_v){
        for(auto hist : signals     ){ 
          hist->SetMaximum( maxh_v );
          hist->SetMinimum( minh_v );
        }
        for(auto hist : backgrounds ) {
          hist->SetMaximum( maxh_v );
          hist->SetMinimum( minh_v );
        }
        for(auto hist : datas ) {
          hist->SetMaximum( maxh_v );
          hist->SetMinimum( minh_v );
        }
      }

      void SetMaximum(){
        double maxh_v = -1.0;
        double minh_v = -1.0;
        for(auto hist : signals     ) maxh_v = TMath::Max(maxh_v, hist->GetBinContent(hist->GetMaximumBin()));
        for(auto hist : backgrounds ) maxh_v = TMath::Max(maxh_v, hist->GetBinContent(hist->GetMaximumBin()));
        for(auto hist : datas       ) maxh_v = TMath::Max(maxh_v, hist->GetBinContent(hist->GetMaximumBin()));
        if( logY ) {
          // minh_v = 0.000001*maxh_v;
          // maxh_v = 100000*maxh_v;
          minh_v = 0.0000001*maxh_v;
          maxh_v = 10000*maxh_v;
        }
        else {
          minh_v = 0.;
          maxh_v = 1.5*maxh_v;
        }
        SetMinMax(minh_v, maxh_v);
      }

      void SetMaximumStack(THStack * hs){
        double maxh_v =  0.0;
        double minh_v = -1.0;
        for(auto hist : backgrounds ) maxh_v += hist->GetBinContent(hist->GetMaximumBin());
        for(auto hist : signals     ) maxh_v  = TMath::Max(maxh_v, hist->GetBinContent(hist->GetMaximumBin()));
        for(auto hist : datas       ) maxh_v  = TMath::Max(maxh_v, hist->GetBinContent(hist->GetMaximumBin()));
        if( logY ) {
          minh_v = 0.000001*maxh_v;
          maxh_v = 100000*maxh_v;
        }
        else {
          minh_v = 0.;
          maxh_v = 1.5*maxh_v;
        }
        SetMinMax(minh_v, maxh_v);
        if( not backgrounds.size() ) return;
        hs->SetMinimum( minh_v );
        hs->SetMaximum( maxh_v );
      }

      void DrawHists(){
        SetMaximum();
             if(backgrounds.size())  backgrounds.at(0)->Draw("hist");
        else if( signals.size()   )      signals.at(0)->Draw("hist");
        else if( datas.size()     )        datas.at(0)->Draw("hist");
        for(auto hist : backgrounds) hist->Draw("same hist");
        for(auto hist : signals)     hist->Draw("same hist");
        for(auto hist : datas)       hist->Draw("same E1");
      }

      void DrawHistsStack(){
        THStack * hs = new THStack("stack", "stack");
        for(auto hist : backgrounds){
          hist->SetFillColor( hist->GetLineColor() );
          hs->Add(hist);
        }
        SetMaximumStack( hs );
        hs->Draw("hist");
        for(auto hist : signals) hist->Draw("same hist");

        if( not hs->GetXaxis() or not hs->GetYaxis() or not hs->GetHistogram() ) return;
        SetStyleHist( hs->GetHistogram() );
        if( signals.size() ){
          hs->GetXaxis()->SetTitle( signals.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( signals.at(0)->GetYaxis()->GetTitle() );
        }
        else if( backgrounds.size() ){
          hs->GetXaxis()->SetTitle( backgrounds.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( backgrounds.at(0)->GetYaxis()->GetTitle() );
        }
        else if( datas.size() ){
          hs->GetXaxis()->SetTitle( datas.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( datas.at(0)->GetYaxis()->GetTitle() );
        }
      }

      void DrawHistsTMVA(){
        THStack * hs = new THStack();
        double sum_integral = 0;
        for(auto hist : backgrounds){
          sum_integral += hist->Integral();
          hist->SetFillColor( hist->GetLineColor() );
          hist->Print();
          hs->Add(hist);
        }
        double alt_sum_integral = 0;
        for(auto hist : datas){
          alt_sum_integral += hist->Integral();
        }
        sum_integral = TMath::Max(sum_integral, alt_sum_integral);
        if( backgrounds.size() + datas.size() < 1 and signals.size() > 0){
          sum_integral = signals.at(0)->Integral();
        }
        //for(auto hist : backgrounds)
        //  hist->Scale(1./sum_integral);
        for(auto hist : signals)
          hist->Scale( 0.50*sum_integral / hist->Integral() );

        SetMaximumStack( hs );
        if( backgrounds.size() ) hs->Draw("hist");
        else {
          cout << "DrawHistsTMVA(): no backgrounds ... " << backgrounds.size() << endl; 
          if( signals.size()   )      signals.at(0)->Draw("hist");
          else if( datas.size()     )   datas.at(0)->Draw("hist");
          else return;
        }

        for(auto hist : signals)
          hist->Draw("same hist");
        for(auto hist : datas)
          hist->Draw("same E1");

        if( not hs->GetXaxis() or not hs->GetYaxis() or not hs->GetHistogram() ) return;
        SetStyleHist( hs->GetHistogram() );
        if( signals.size() ){
          hs->GetXaxis()->SetTitle( signals.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( signals.at(0)->GetYaxis()->GetTitle() );
        }
        else if( backgrounds.size() ){
          hs->GetXaxis()->SetTitle( backgrounds.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( backgrounds.at(0)->GetYaxis()->GetTitle() );
        }
        else if( datas.size() ){
          hs->GetXaxis()->SetTitle( datas.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( datas.at(0)->GetYaxis()->GetTitle() );
        }
      }

      void DrawHistsOverset(){
        THStack * hs = new THStack();
        double sum_integral = 0;
        for(auto hist : backgrounds){
          sum_integral += hist->Integral();
          hist->SetFillColor( hist->GetLineColor() );
          hist->Print();
          hs->Add(hist);
        }
        //for(auto hist : backgrounds)
        //  hist->Scale(1./sum_integral);
        vector<TH1D *> hs_signals;

        for(auto hist : signals){
          hist->Scale( signal_scale );

          TH1D * hs_signal = (TH1D *) hist->Clone();
          for(auto hist : backgrounds) hs_signal->Add(hist);
          hs_signals.push_back( hs_signal );
        }

        SetMaximumStack( hs );
        hs->Draw("hist");

        for(auto hist : hs_signals){
          hist->Draw("same hist");
        }
        for(auto hist : datas)
          hist->Draw("same E1");

        if( not hs->GetXaxis() or not hs->GetYaxis() or not hs->GetHistogram() ) return;
        SetStyleHist( hs->GetHistogram() );
        if( signals.size() ){
          hs->GetXaxis()->SetTitle( signals.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( signals.at(0)->GetYaxis()->GetTitle() );
        }
        else if( backgrounds.size() ){
          hs->GetXaxis()->SetTitle( backgrounds.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( backgrounds.at(0)->GetYaxis()->GetTitle() );
        }
        else if( datas.size() ){
          hs->GetXaxis()->SetTitle( datas.at(0)->GetXaxis()->GetTitle() );
          hs->GetYaxis()->SetTitle( datas.at(0)->GetYaxis()->GetTitle() );
        }
      }

      void DrawHists2D(){
        for(auto hist : backgrounds){
          hist->SetMarkerColor( 1 );
          hist->SetLineColor( 1 );
        }
        for(auto hist : signals)
          hist->SetMarkerColor( hist->GetLineColor() );

             if(backgrounds.size()) backgrounds.at(0)->Draw( corr_draw_option.c_str() );
        else if( signals.size())    signals.at(0)->Draw( corr_draw_option.c_str()  );

        for(auto hist : backgrounds) hist->Draw( ("same " + corr_draw_option).c_str() );
        for(auto hist : signals) hist->Draw( ("same " + corr_draw_option).c_str() );
        // TODO data ?
      }

      TLegend * GetLegend(float x1=0.55, float y1=0.65, float x2=0.90, float y2=0.88){
        TLegend * legend = new TLegend(x1,y1,x2,y2);
        legend->SetFillColor(0);
        legend->SetFillStyle(3001);
        legend->SetLineColor(0);
        legend->SetTextFont(font) ;
        for(auto hist : datas)
          legend->AddEntry(hist, hist->GetTitle(), "p");
        for(auto hist : signals)
          legend->AddEntry(hist, hist->GetTitle(), "l");
        for(auto hist : backgrounds)
          legend->AddEntry(hist, hist->GetTitle(), "f");
        return legend;
      }

      TLegend * GetLegend2D(float x1=0.55, float y1=0.75, float x2=0.90, float y2=0.88){
        TLegend * legend = new TLegend(x1,y1,x2,y2);
        legend->SetLineColor(1);
        legend->SetLineWidth(3);
        legend->SetTextFont(font) ;
        gStyle->SetLegendBorderSize(3);
        for(auto hist : signals)
          legend->AddEntry(hist, hist->GetTitle(), "lp");
        if(backgrounds.size())
          legend->AddEntry(backgrounds.at(0), "background", "lp");
        return legend;
      }

      TLegend * GetLegendCutStyle(string cut_text, float x1=0.55, float y1=0.65, float x2=0.90, float y2=0.88){
        TLegend * legend = new TLegend(x1,y1,x2,y2);
        legend->SetFillColor(0);
        legend->SetFillStyle(3001);
        legend->SetLineColor(0);
        legend->SetTextFont(font) ;
        legend->AddEntry(signals.at(0), signals.at(0)->GetTitle(), "l");
        legend->AddEntry(backgrounds.at(0), backgrounds.at(0)->GetTitle(), "l");
        legend->AddEntry((TObject*)0, cut_text.c_str(), "");
        legend->AddEntry(signals.at(1), signals.at(1)->GetTitle(), "l");
        legend->AddEntry(backgrounds.at(1), backgrounds.at(1)->GetTitle(), "l");
        return legend;
      }

      TLatex * GetText(string text_src, float x = 0.23, float y = 0.82, float text_size = 0.044){
        cout << "THIS " << endl;
        TLatex * text = new TLatex(x, y, text_src.c_str());
        text->SetNDC(kTRUE) ;
        text->SetTextSize(text_size) ;
        text->SetTextFont(font) ;
        return text;
      }

      TLatex * GetLeftText(string text_src){  return GetText(text_src, 0.16, 0.82, 0.034); }
      TLatex * GetRightText(string text_src){ return GetText(text_src, 0.45, 0.92, 0.040); }

      TCanvas * GetCanvas(string name, int size_x = 600, int size_y = 600){
        TCanvas * canvas = new TCanvas(name.c_str(), name.c_str(), size_x, size_y);
        canvas->SetTicks(1,1);
        canvas->SetLeftMargin(0.14); 
        canvas->SetRightMargin(0.08); 
        canvas->SetBottomMargin(0.20); 
        gStyle->SetOptStat(0000000);
        gStyle->SetTextFont(font);
        gStyle->SetOptTitle(0);
        if(logY) canvas->SetLogy();
        return canvas;
      }

      TCanvas * GetCanvas2D(string name, int size_x = 800, int size_y = 600){
        TCanvas * canvas = new TCanvas(name.c_str(), name.c_str(), size_x, size_y);
        canvas->SetRightMargin(0.18);
        canvas->SetLeftMargin(0.22);
        canvas->SetBottomMargin(0.18);
        canvas->SetTopMargin(0.12);
        return canvas;
      }


      void Add(TMP_hist_type* hist, string type){
        if(type == "S")   signals.push_back( hist );
        else if(type == "B") backgrounds.push_back( hist );
        else if(type == "D") datas.push_back( hist );
      }

      void AddCummulative(TMP_hist_type* hist, string type, int index=0){
        if(type == "S"){ 
          if(index >= signals.size()) signals.push_back( hist ); 
          else                        signals.at(index)->Add( hist );
        }
        else if(type == "B"){ 
          if(index >= backgrounds.size()) backgrounds.push_back( hist ); 
          else                            backgrounds.at(index)->Add( hist );
        }
        else if(type == "D"){ 
          if(index >= datas.size()) datas.push_back( hist ); 
          else                      datas.at(index)->Add( hist );
        }
      }
      
      void AddProcess(TMP_hist_type* hist, string type, string process_name){
        auto it = processes.find( process_name );
        if(it != processes.end()){
          AddCummulative(hist, type, it->second);
          return;
        }
        int index = -1;
        Add(hist, type);
        if(type == "S")      index = signals.size()-1;
        else if(type == "B") index = backgrounds.size() - 1;
        else if(type == "D") index = datas.size() - 1;
        
        processes[ process_name ] = index;
      }
      map<string, int> processes; 
  };

  template<class T> TCanvas * draw_hists_CMS( HistDrawer<T> * drawer, string path, string name, string label, string extra_title = "", string mode = "default"){
    set_style_FCC();
    string leftText  = "";
    string rightText = "";
           rightText = "CMS Preliminary";
           rightText = "";
  
    cout << name << endl;
    drawer->Print();

    TCanvas * canvas;
    if(mode == "default" or mode == "default nolog"){
      drawer->logY = true;
      if(mode == "default nolog") drawer->logY = false;
      drawer->label_x = label;
      drawer->SetStyle();
      canvas = drawer->GetCanvas( name );
      drawer->DrawHists();
      auto legend = drawer->GetLegend();
      auto ltext  = drawer->GetLeftText( leftText );
      auto rtext  = drawer->GetRightText( rightText );
      auto etext  = drawer->GetText(extra_title, 0.23, 0.92);
      legend->Draw();
      ltext->Draw() ;
      rtext->Draw() ;
      etext->Draw() ;
    } else if(mode == "tmva"){
      drawer->logY = false;
      drawer->label_x = label;
      cout << " .... set style" << endl;
      drawer->SetStyle();
      cout << " .... get canvas" << endl;
      canvas = drawer->GetCanvas( name );
      cout << " .... DRAW" << endl;
      drawer->DrawHistsTMVA();
      cout << " .... DRAW ok" << endl;
      auto legend = drawer->GetLegend();
      auto ltext  = drawer->GetLeftText( leftText );
      auto rtext  = drawer->GetRightText( rightText );
      auto etext  = drawer->GetText(extra_title, 0.23, 0.92);
      legend->Draw();
      ltext->Draw() ;
      rtext->Draw() ;
      etext->Draw() ;
    } else if(mode == "overset"){
      drawer->logY = false;
      drawer->label_x = label;
      drawer->SetStyle();
      canvas = drawer->GetCanvas( name );
      drawer->DrawHistsOverset();
      auto legend = drawer->GetLegend();
      auto ltext  = drawer->GetLeftText( leftText );
      auto rtext  = drawer->GetRightText( rightText );
      auto etext  = drawer->GetText(extra_title, 0.23, 0.92);
      legend->Draw();
      ltext->Draw() ;
      rtext->Draw() ;
      etext->Draw() ;
    } else if (mode == "correlation"){
      drawer->logY = false;
      drawer->SetStyle();
      canvas = drawer->GetCanvas2D( name );
      drawer->DrawHists2D();
      auto legend = drawer->GetLegend2D();
      auto ltext  = drawer->GetLeftText( extra_title );
      auto rtext  = drawer->GetRightText( rightText );
      auto etext  = drawer->GetText("", 0.23, 0.92);
      legend->Draw();
      ltext->Draw() ;
      rtext->Draw() ;
      etext->Draw() ;
    } else if (mode == "cut"){
      drawer->logY = true;
      drawer->label_x = label;
      drawer->SetStyle();
      canvas = drawer->GetCanvas( name );
      drawer->DrawHists();
      auto legend = drawer->GetLegendCutStyle( extra_title );
      auto ltext  = drawer->GetLeftText( leftText );
      auto rtext  = drawer->GetRightText( rightText );
      // auto etext  = drawer->GetText(extra_title, 0.23, 0.92);
      legend->Draw();
      ltext->Draw() ;
      rtext->Draw() ;
      //etext->Draw() ;
    } else {
      cout << "draw_hists_FCC(): wrong draw mode = " << mode << endl;;
    }
    canvas->SetTicks(1,1);

    canvas->RedrawAxis();
    canvas->GetFrame()->SetBorderSize( 12 );
    canvas->Modified();
    canvas->Update();
    canvas->Print( (path + "/" + name).c_str() );
    return canvas;
  }

  


  //==========================================================================================================================
  /*void draw_hist_from_two_ttrees(string var_name, TTree * tree_1, TTree * tree_2, string label_1, string label_2, string weight_1="1", string weight_2="1"){
    // do not display any of the standard histogram decorations
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0); //("m")
    gStyle->SetOptFit(0);

    TTreeFormula var_1f(var_name.c_str(), var_name.c_str(), tree_1);
    TTreeFormula var_2f(var_name.c_str(), var_name.c_str(), tree_2);

    TTreeFormula weight_1f(weight_1.c_str(), weight_1.c_str(), tree_1);
    TTreeFormula weight_2f(weight_2.c_str(), weight_2.c_str(), tree_2);
  }*/

  void draw_hists_from_two_ttrees(TTree * tree_1, TTree * tree_2, string label_1, string label_2, string weight_1="1", string weight_2="1"){
    // do not display any of the standard histogram decorations
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0); //("m")
    gStyle->SetOptFit(0);

    int h_counter = 0;

    /*
    TTreeFormula weight_1_f(weigh_1.c_str(), weigh_1.c_str(), tree_1);
    if( weight_1_f.GetNdim()==0){
      cout << "Don't have a branch with name = " << weigh_1 << " " << tmva_name << endl;
    }

    TTreeFormula weight_2_f(weigh_2.c_str(), weigh_2.c_str(), tree_2);
    if( weight_2_f.GetNdim()==0){
      cout << "Don't have a branch with name = " << weigh_2 << " " << tmva_name << endl;
    }
    */

    size_t n = tree_1->GetListOfBranches()->GetEntries();
    cout << "draw_hists_from_two_ttrees(): Input ttrees " << tree_1 << " " << tree_2 << endl;
    cout << " Process TTree " << tree_1->GetName() << endl;
    TObjArray * branches_1 = tree_1->GetListOfBranches();
    TObjArray * branches_2 = tree_2->GetListOfBranches();
    for( size_t i = 0; i < branches_1->GetEntries(); ++ i ) {
      TBranch *subbr_1 = dynamic_cast<TBranch*>(branches_1->At(i));
      string name_1 = subbr_1->GetName();
      cout << "  process branch \"" << subbr_1->GetName() << endl;

      TBranch *subbr_2 = nullptr;
      for( size_t j = 0; j < branches_2->GetEntries(); ++ j ) {
        TBranch *subbr_temp = dynamic_cast<TBranch*>(branches_2->At(j));
        string name_2 = subbr_temp->GetName();
        if( name_1 != name_2 ) continue;
        subbr_2 = subbr_temp;
        break;
      }

      if( subbr_2 == nullptr ) {
        cout << "  skip as second ttree dont have such variable" << endl;
        continue;
      }

      vector<string> operators;
      vector<string> names;
      if(name_1.find("_tlv") != string::npos ){
        operators.push_back( ".Pt()"  );
        operators.push_back( ".Eta()" );
        operators.push_back( ".Phi()" );
        operators.push_back( ".M()" );
        names.push_back( "_Pt"  );
        names.push_back( "_Eta"  );
        names.push_back( "_Phi"  );
        names.push_back( "_M"  );
      } else {
        operators.push_back("");
        names.push_back("");
      }
  
      for(int k = 0; k < operators.size(); k++){
        h_counter++;

        string oper = operators.at(k);
        string postfix  = names.at(k);
        
        tree_1->Draw( (name_1 + oper +" >> h_a_1" + to_string(h_counter) ).c_str(), weight_1.c_str() );
        TH1* h_c_1 = ((TH1*)gPad->GetPrimitive( ("h_a_1" + to_string(h_counter)).c_str() ) ); // ->Clone(); 
        TH1* h_a_1 = (TH1*) h_c_1->Clone(); 

        tree_2->Draw( (name_1 + oper +" >> h_a_2" + to_string(h_counter)).c_str(), weight_2.c_str() );
        TH1* h_c_2 = ((TH1*)gPad->GetPrimitive( ("h_a_2" + to_string(h_counter)).c_str() ) ); // ->Clone(); 
        TH1* h_a_2 = (TH1*) h_c_2->Clone(); 

        double min = TMath::Min( h_a_1->GetXaxis()->GetXmin(), h_a_2->GetXaxis()->GetXmin() );
        double max = TMath::Max( h_a_1->GetXaxis()->GetXmax(), h_a_2->GetXaxis()->GetXmax() );
        cout << min << " " << max << endl;

        TH1D * h_b_1 = new TH1D("h_b_1", label_1.c_str(), h_a_1->GetNbinsX(), min, max);
        TH1D * h_b_2 = new TH1D("h_b_2", label_2.c_str(), h_a_1->GetNbinsX(), min, max);

        tree_1->Draw( (name_1 + oper +" >> h_b_1").c_str(), weight_1.c_str() );
        tree_2->Draw( (name_1 + oper +" >> h_b_2").c_str(), weight_2.c_str() );

        h_b_2->SetLineColor(2);
        TCanvas * canv = new TCanvas("canv", "canv", 640, 480);
        h_b_1->Draw("hist");
        h_b_2->Draw("hist same");
        h_b_1->GetXaxis()->SetTitle( (name_1 + oper).c_str() );

        canv->BuildLegend();
        

        canv->Print( (name_1 + postfix + ".png").c_str() );
      }
    }
  }

  //==========================================================================================================================
  void draw_correlations_from_two_ttrees(TTree * tree_1, TTree * tree_2, vector<string> variables, vector<string> labels){
    int h_counter = 0;
    cout << "draw_correlations_from_two_ttrees(): Input ttrees " << tree_1 << " " << tree_2 << endl;

    cout << variables.size() << " " << labels.size() << endl;

    for(int i = 0; i < variables.size(); i++){
      for(int j = i+1; j < variables.size(); j++){
        h_counter++;
        string v1 = variables.at( i );
        string v2 = variables.at( j );

        string lab1 = labels.at( i );
        string lab2 = labels.at( j );

        cout << v1 << " " << lab1 << " " << v2 << " " << lab2 << endl;

        tree_1->Draw( (v1 + ":" +v2 + " >> h_a_1" + to_string(h_counter) ).c_str() );
        TH2* h_c_1 = ((TH2*)gPad->GetPrimitive( ("h_a_1" + to_string(h_counter)).c_str() ) );
        TH2* h_a_1 = (TH2*) h_c_1->Clone(); 

        tree_2->Draw( (v1 + ":" +v2 + " >> h_a_2" + to_string(h_counter) ).c_str() );
        TH2* h_c_2 = ((TH2*)gPad->GetPrimitive( ("h_a_2" + to_string(h_counter)).c_str() ) );
        TH2* h_a_2 = (TH2*) h_c_2->Clone(); 

        double min_x = TMath::Min( h_a_1->GetXaxis()->GetXmin(), h_a_2->GetXaxis()->GetXmin() );
        double max_x = TMath::Max( h_a_1->GetXaxis()->GetXmax(), h_a_2->GetXaxis()->GetXmax() );
        double min_y = TMath::Min( h_a_1->GetYaxis()->GetXmin(), h_a_2->GetYaxis()->GetXmin() );
        double max_y = TMath::Max( h_a_1->GetYaxis()->GetXmax(), h_a_2->GetYaxis()->GetXmax() );
        cout << min_x << " " << max_x << " " << min_y << " " << max_y << endl;

        TH2D * h_b_1 = new TH2D("h_b_1", "signal",     h_a_1->GetNbinsX(), min_x, max_x, h_a_1->GetNbinsY(), min_y, max_y);
        TH2D * h_b_2 = new TH2D("h_b_2", "background", h_a_1->GetNbinsX(), min_x, max_x, h_a_1->GetNbinsY(), min_y, max_y);
        tree_1->Draw( (v1 + ":" +v2 + " >> h_b_1" ).c_str() );
        tree_2->Draw( (v1 + ":" +v2 + " >> h_b_2" ).c_str() );

        h_b_1->SetMarkerStyle( 20 );
        h_b_2->SetMarkerStyle( 21 );

        h_b_1->SetMarkerSize( 0.25 );
        h_b_2->SetMarkerSize( 0.25 );

        HistDrawer<TH2D> * drawer = new HistDrawer<TH2D>();
        drawer->Add(h_b_1, "S");
        // drawer->Add(h_b_2, "B");

        drawer->label_y = lab1;
        drawer->label_x = lab2;

        drawer->corr_draw_option = "COL";
        //draw_hists_CMS( HistDrawer<T> * drawer, string path, string name, string label, string extra_title = "", string mode = "default"){
        draw_hists_CMS(drawer, ".", to_string(h_counter) + ".png", "", "", "correlation");
      }
      break;
    }
  }

};

#endif



