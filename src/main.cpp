#include "ConverterJSON.h"


int main(){
    // ConverterJSON converter;
    // InvertedIndex i;
    // i.UpdateDocumentBase(converter.GetDocumets());
    // SearchServer server(i);
    // converter.putAnswers(server.search(converter.GetRequests()));

    ConverterJSON converter;

    InvertedIndex idx(converter.GetNumThread());
    idx.SetCountRelev(converter.GetResponsesLimit());

    idx.UpdateDocumentBase(converter.GetDocumets());

    SearchServer srv(idx, converter.GetNumThread());
    converter.putAnswers(srv.search(converter.GetRequests()));



    return 0;
}