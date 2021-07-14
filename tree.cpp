#include "tree.h"
#include "QDebug"
#include <QTextStream>
#include "global_objects.h"
#include "QMap"
#include "QQueue"

Node::Node(int i , QString t , QString d , QVector<QString> at , QVector<QString> av , int l)
{
        id = i;
        tag = t;
        data = d;
        level = l;
        attrTag = at;
        attrVal = av;
}

Node::~Node()
{
   /* for ( int i = 0 ; i < childern.size() ; i++ )
    {
        delete childern[i];
    }
    delete this;*/
}



Tree::Tree()
{
    root = NULL;
}

Tree::~Tree()
{
    root = NULL;
}

void Tree::insert (Node* leaf , int id , QString tag , QString data , QVector<QString> attrTag , QVector<QString> attrVal , int parent, int level)
{
    Node* n = new Node(id , tag , data , attrTag , attrVal , level);

    if(leaf->id == parent)
        leaf->childern.push_back(n);
    else
    {
        for(int i = 0 ; i < leaf->childern.size(); i++)
            insert(leaf->childern[i],id,tag,data, attrTag , attrVal,parent,level);
    }
}

void Tree::insert(int id , QString tag , QString data, QVector<QString> attrTag , QVector<QString> attrVal, int parent, int level)
{
    if(root != NULL)
        insert(root,id,tag,data,attrTag,attrVal,parent,level);

    else
    {
        root = new Node(id,tag,data,attrTag,attrVal,level);
    }
}


void Tree::postOrder(Node* t)
    {
        if ( t  != NULL )
        {
            for(int i = 0 ; i < t->level ; i++)
            xml +="\t";

            xml +='<'+t->tag+"";

            for (int i = 0 ; i < t->attrTag.size(); i++){
            if(t->attrVal[i] != NULL)
                xml += " " + t->attrTag[i]+"=\""+t->attrVal[i]+"\"";
            }

            xml +=">";

            xml += "\n";
            if(t->data != NULL)
            {
                for(int i = 0 ; i < t->level ; i++)
                xml +="\t       ";
            }
            xml +=t->data;
            if(t->data != NULL)
                xml += "\n";
            for ( int i = 0 ; i < t->childern.size() ; i++)
            {
                postOrder(t->childern[i]);
            }
            for(int i = 0 ; i < t->level ; i++)
            xml +="\t";
            xml += "</"+t->tag+'>'+"\n";
        }

    }
void Tree::postOrderMini(Node *t)
    {

    if ( t  != NULL )
    {

        xml +='<'+t->tag+"";

        for (int i = 0 ; i < t->attrTag.size(); i++){
        if(t->attrVal[i] != NULL)
            xml += " " + t->attrTag[i]+"=\""+t->attrVal[i]+"\"";
        }

        xml +=">";

        if(t->data != NULL)
        {
            for(int i = 0 ; i < t->level ; i++);
        }
        xml +=t->data;

        for ( int i = 0 ; i < t->childern.size() ; i++)
        {
            postOrderMini(t->childern[i]);
        }

        xml += "</"+t->tag+'>';
    }

    }

void Tree::postOrder()
    {
    xml = "";

        postOrder(root);
    }


void Tree::postOrderMini()
    {
    xml = "";

        postOrderMini(root);
    }
bool figo = 0;
bool first = 1;
QMap<QString,int> m;
QVector<int>final;
void Tree::postOrderJson(Node* t)
    {

        if ( t  != NULL )
        {

            for(int i = 0 ; i < t->level ; i++)
            json +="\t";

            if(m[t->tag] == 0 || t->tag == "frame")
            {
                json +="\""+t->tag+"\": ";
                if( t->data == NULL )

                    if(t->tag !="frame")
                final.push_back(t->childern.size());
            }

            if(m[t->tag] != 0) {

                if(t->tag == "frame") ;
                else json +=",";

                first = 0;

            }

            m[t->tag]++;

            if (t->data != NULL)
            {
            json +="\""+t->data+"\"";
            m[t->tag]--;
            }

            if(t->data == NULL)
            {
            if(m[t->tag] != 0 && first == 1 && t->id != 1) xml += "[{ \n";
            else json += "{ \n";
            }
            for ( int i = 0 ; i < t->childern.size() ; i++)
            {
                if( i == t->childern.size() - 1)
                {
                    figo = 1;
                }
                postOrderJson(t->childern[i]);
            }

            if(t->data != NULL && figo != 1)
            json += ",";

            figo = 0;

        //
        }
        if(json.right(1) != "\n")
        json += "\n";

        if(t->data == NULL){
        for(int i = 0 ; i < t->level ; i++)
        json +="\t";
        if(final.size() > 2){

        if(m[t->tag] == final[final.size() - 2] )
        {
            json+= "    }]\n";
            final.pop_back();
        }
        }
        else json += "    }\n";

    first = 1;

        }
    }

void Tree::postOrderJson()
    {
    json = "";
    json += "{ \n";
        postOrderJson(root);
    json += "}";
    }

