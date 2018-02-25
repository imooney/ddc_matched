//  newplot.C
//  Created by Isaac Mooney on 5/5/17.

{
    //uncut constituents
    gROOT->ProcessLine(".L uncutConsClass.C");
    uncutConsClass *a = new uncutConsClass;
    a->Loop();
    delete a; a = NULL;
    
    //charged uncut constituents
    gROOT->ProcessLine(".L c_uncutConsClass.C");
    c_uncutConsClass *b = new c_uncutConsClass;
    b->Loop();
    delete b; b = NULL;
    
    //raw jets
    gROOT->ProcessLine(".L uncutClass.C");
    uncutClass *c = new uncutClass;
    c->Loop();
    delete c; c = NULL;
    
    //charged raw jets
    gROOT->ProcessLine(".L cuncutClass.C");
    cuncutClass *d = new cuncutClass;
    d->Loop();
    delete d; d = NULL;
    
    //jets with particle cuts
    gROOT->ProcessLine(".L cutClass.C");
    cutClass *e = new cutClass;
    e->Loop();
    delete e; e = NULL;
    
    //charged jets with particle cuts
    gROOT->ProcessLine(".L ccutClass.C");
    ccutClass *f = new ccutClass;
    f->Loop();
    delete f; f = NULL;
    
    //jets with both sets of cuts
    gROOT->ProcessLine(".L cut2Class.C");
    cut2Class *g = new cut2Class;
    g->Loop();
    delete g; g = NULL;
    
    //charged jets with both sets of cuts
    gROOT->ProcessLine(".L ccut2Class.C");
    ccut2Class *h = new ccut2Class;
    h->Loop();
    delete h; h = NULL;
    
    //jets with both sets of cuts + efficiency correction
    gROOT->ProcessLine(".L efficiencyClass.C");
    efficiencyClass *i = new efficiencyClass;
    i->Loop();
    delete i; i = NULL;
    
    //charged jets with both sets of cuts + efficiency correction
    gROOT->ProcessLine(".L cefficiencyClass.C");
    cefficiencyClass *j = new cefficiencyClass;
    j->Loop();
    delete j; j = NULL;
    
    //leading jet pT spectra
    gROOT->ProcessLine(".L leadPtClass.C");
    leadPtClass *k = new leadPtClass;
    k->Loop();
    delete k; k = NULL;
    
    //number and pT differences
    gROOT->ProcessLine(".L DiffClass.C");
    DiffClass *l = new DiffClass;
    l->Loop();
    delete l; l = NULL;
    
    //tests
    gROOT->ProcessLine(".L testsClass.C");
    testsClass *m = new testsClass;
    m->Loop();
    delete m; m = NULL;
    
    //tests
    gROOT->ProcessLine(".L efficConsClass.C");
    efficConsClass *n = new efficConsClass;
    n->Loop();
    delete n; n = NULL;
    
    //tests
    gROOT->ProcessLine(".L c_efficConsClass.C");
    c_efficConsClass *o = new c_efficConsClass;
    o->Loop();
    delete o; o = NULL;
    
    //tests
    gROOT->ProcessLine(".L cut2ConsClass.C");
    cut2ConsClass *p = new cut2ConsClass;
    p->Loop();
    delete p; p = NULL;
    
    //tests
    gROOT->ProcessLine(".L c_cut2ConsClass.C");
    c_cut2ConsClass *q = new c_cut2ConsClass;
    q->Loop();
    delete q; q = NULL;
    
    
}
