#include "information.h"


GeneralInformation::GeneralInformation() {
    this->title = "";
    this->url = "";
}


AtomEntryInformation::AtomEntryInformation() {
    this->last_updated = "";
    this->published = "";
}


void AtomEntryInformationSummary::print_info(bool sep_newline) {
    for (int i = 0; i < entry_information.size(); i++) {
        AtomEntryInformation entry = this->entry_information[i];
        if (entry.title == "")
            cout << "Empty title" << endl;
        else
            cout << entry.title << endl;

        // print entry authors if any
        if (entry.authors.size() > 0) {
            for (string author: entry.authors) {
                cout << "Autor: " << author << endl;
            }
        // otherwise print dublin core authors
        } else if (entry.dc_creators.size() > 0) {
            for (string author: entry.dc_creators) {
                cout << "Autor: " << author << endl;
            }
        // otherwise print authors in entry source if any
        } else if (entry.source_authors.size() > 0) {
            for (string source_author: entry.source_authors) {
                cout << "Autor: " << source_author << endl;
            }
        // otherwise print feed authors
        } else if (this->global_authors.size() > 0) {
            for (string feed_author: this->global_authors) {
                cout << "Autor: " << feed_author << endl;
            }
        }

        // print URL
        if (entry.url != "") {
            cout << "URL: " << entry.url << endl;
        // print alternate URL
        } else if (entry.alternate_url != "") {
            cout << "URL: " << entry.alternate_url << endl;
        }

        // print update date
        if (entry.last_updated != "") {
            cout << "Aktualizace: " << entry.last_updated << endl;
        } else if (entry.published != "") {
            cout << "Aktualizace: " << entry.published << endl;
        } else if (entry.dc_date != "") {
            cout << "Aktualizace: " << entry.dc_date << endl;
        }

        // separate the entry by newline
        if (sep_newline) {
            if (i != entry_information.size()-1) {
                cout << endl;
            }
        }
    }
}


RssItemInformation::RssItemInformation() {
    this->author = "";
    this->last_updated = "";
    this->guid_permalink = "";
}


void RssItemInformationSummary::print_info(bool sep_newline) {
    for (int i = 0; i < item_information.size(); i++) {
        RssItemInformation item = this->item_information[i];
        if (item.title == "")
            cout << "Empty title" << endl;
        else
            cout << item.title << endl;

        // print item author if any
        if (item.author != "") {
            cout << "Autor: " << item.author << endl;
        // otherwise print dublin core item author
        } else if (item.dc_creator != "") {
            cout << "Autor: " << item.dc_creator << endl;
        // otherwise print dublin core channel creator
        } else if (this->dc_creator != "") {
            cout << "Autor: " << this->dc_creator << endl;
        }

        // print URL
        if (item.url != "") {
            cout << "URL: " << item.url << endl;
        // print URL from GUID
        } else if (item.guid_permalink != "") {
            cout << "URL: " << item.guid_permalink << endl;
        }

        // print update date
        if (item.last_updated != "") {
            cout << "Aktualizace: " << item.last_updated << endl;
        } else if (item.dc_date != "") {
            cout << "Aktualizace: " << item.dc_date << endl;
        }

        // separate the entry by newline
        if (sep_newline) {
            if (i != item_information.size()-1) {
                cout << endl;
            }
        }
    }
}
