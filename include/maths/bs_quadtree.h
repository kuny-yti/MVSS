#ifndef BS_bs_quadtree_H
#define BS_bs_quadtree_H
#include "bs_define.h"
#include "bs_aabb.h"


template<typename T>
class bs_quadtree
{
private:
    struct node;
    struct base
    {
        node*       parent_;

        bs_aabb  bounds_;
        bs_aabb  extbounds_;

        float   threshold_;
        bool    isLeaf_;

        size_t  finishedChildren_;

        base(const bs_aabb& bounds, float threshold, bool isLeaf, node* parent)
          : parent_(parent),
            bounds_(bounds),
            extbounds_(bounds),
            threshold_(threshold),
            isLeaf_(isLeaf),
            finishedChildren_(0)
        {}

        virtual ~base() {}

        virtual bs_aabb* insert(T* obj, float weight) = 0;
    };

    struct node : public base
    {
        friend class bs_quadtree<T>::iterator;
        friend class bs_quadtree<T>::leaf_iterator;
        friend class bs_bs_quadtree<T>;

        base*   children_[4];

    public:

        node(const bs_aabb& bounds, float threshold, node* parent)
          : base(bounds, threshold, false, parent)
        {
            for (size_t i = 0; i < 4; ++i)
                children_[i] = bsNew (leaf(calculateChildBounds(bounds, i), threshold, this));
        }

        virtual ~node() {
            for (size_t i = 0; i < 4; ++i)
                if (children_[i])
                    bsDelete (children_[i]) ;
        }

        virtual bs_aabb* insert(T* obj, float weight);

    };

public:
    struct leaf : public base
    {
        bs_vector<T*>     objs_;
        bs_vector<float>  weights_;
        float             weightSum_;

        leaf(const bs_aabb& bounds, float threshold, node* parent)
          : base(bounds, threshold, true, parent),
            weightSum_(0)
        {}
        virtual ~leaf() {}

        virtual bs_aabb* insert(T* obj, float weight);
    };

    class leaf_iterator {

    private:
        leaf* leaf_;

        static leaf* iterate(base* child);

    public:

        leaf_iterator(leaf* leaf)
          : leaf_(leaf)
        {}

        leaf_iterator& operator ++ () {
            leaf_ = iterate(leaf_);
            return *this;
        }

        leaf_iterator  operator ++(int) {
            leaf_iterator iter = *this;
            leaf_ = iterate(leaf_);
            return iter;
        }

        bool operator == (const leaf_iterator& iter) {
            return leaf_ == iter.leaf_;
        }

        bool operator != (const leaf_iterator& iter) {
            return leaf_ != iter.leaf_;
        }

        leaf* operator&() {
            return leaf_;
        }

        leaf* operator->() {
            return leaf_;
        }

        leaf& operator*() {
            return *leaf_;
        }
    };

    class iterator
    {
    private:
        base* node_;
        const base* root_;

        base* iterate(base* child);

    public:

        iterator(base* node)
          : node_(node), root_(node)
        {}

        ///  preincrement operator
        iterator& operator ++ () {
            node_ = iterate(node_);
            return *this;
        }

        /// postincrement operator
        iterator  operator ++(int) {
            iterator iter = *this;
            node_ = iterate(node_);
            return iter;
        }

        bool operator == (const iterator& iter) {
            return node_ == iter.node_;
        }

        bool operator != (const iterator& iter) {
            return node_ != iter.node_;
        }

        /// similar to the increment-operators, but skips an entire branch
        void skip();

        /// adress operator
        base* operator&() {
            return node_;
        }

        base* operator->() {
            return node_;
        }

        base& operator*() {
            return *node_;
        }
    };

    enum {
        Quadrant_TopLeft = 0,
        Quadrant_TopRight,
        Quadrant_BottomLeft,
        Quadrant_BottomRight
    };

    bs_quadtree(const bs_aabb& bounds, float threshold)
        : rootn(bsNew (node(bounds, threshold, NULL)))
    {}

    ~bs_quadtree() {
        bsDelete (rootn);
    }

    void insert(T* obj, float weight) {
        rootn->insert(obj, weight);
    }

    T* find(float x, float y);

    static bs_aabb calculateChildBounds(const bs_aabb& bounds, size_t childIndex);

    leaf_iterator leaf_begin() {

        node* tmpnode;
        tmpnode = rootn;
        while (!(tmpnode->children_[0]->isLeaf_))
            tmpnode = (node*)(tmpnode->children_[0]);

        return leaf_iterator((leaf*)(tmpnode->children_[0]));
    }

    leaf_iterator leaf_end() {
        return leaf_iterator(NULL);
    }

    iterator begin() {
        return iterator(rootn);
    }

    iterator end() {
        return iterator(NULL);
    }

    protected:
        node*    rootn;

    public:
        node* root(){ return rootn;}

};


template<typename T>
bs_aabb bs_quadtree<T>::calculateChildBounds(const bs_aabb& bounds, size_t childIndex)
{
    // calculate new bounds
    vec3 llf = bounds.min();
    vec3 urb = bounds.max();

    float xdelta = urb.x - llf.x;
    float zdelta = urb.z - llf.z;

    xdelta *= 0.5f;
    zdelta *= 0.5f;

    if (childIndex == 0 || childIndex == 1)
        urb.z -= zdelta;
    if (childIndex == 2 || childIndex == 3)
        llf.z += zdelta;
    if (childIndex == 0 || childIndex == 2)
        urb.x -= xdelta;
    if (childIndex == 1 || childIndex == 3)
        llf.x += xdelta;

    return bs_aabb(llf, urb);
}

template<typename T>
bs_aabb* bs_quadtree<T>::node::insert(T* obj, float weight) {
    //bsASSERT(obj->bounds().isDefined() || obj->getBounds().onlyPoint(), "Bounding Box of inserted object not defined");

    for (size_t i = 0; i < 4; ++i)
    {
        // Must be inserted here?
        if ( children_[i]->bounds_.inside_xz(obj->bounds().center()) )
        {
            // Is the child a leaf?
            if (children_[i]->isLeaf_)
            {
                // Is the threshold exceeded?
                if (((leaf*) children_[i])->weightSum_ + weight > base::threshold_)
                {
                    // Yes? - Change to QuadNode and insert contents there
                    // Store old values
                    bs_vector<T*>    objs    = ((leaf*) children_[i])->objs_;
                    bs_vector<float> weights = ((leaf*) children_[i])->weights_;
                    // destroy node
                    bsDelete ((leaf*) children_[i]);
                    // create node
                    children_[i] = bsNew (node(calculateChildBounds(base::bounds_, i),
                                               base::threshold_, this)); // this is the parent

                    // insert object
                    //bsASSERT(objs.size() == weights.size(), "The size must be equal.");
                    typename bs_vector<T*>::iterator iterObjs    = objs.begin();
                    bs_vector<float>::iterator iterWeights = weights.begin();
                    while (iterObjs != objs.end())
                    {
                        base::extbounds_.append (*children_[i]->insert( *iterObjs, *iterWeights) );
                        ++iterObjs;
                        ++iterWeights;
                    }
                }
            }
            // insert the actual element at all events
            base::extbounds_.append(*children_[i]->insert(obj, weight) );
        }
    }


    bs_aabb& bounds = base::extbounds_;
    bs_aabb* result = &bounds;

    return result;
}

template<typename T>
bs_aabb* bs_quadtree<T>::leaf::insert(T* obj, float weight)
{
    objs_.   push_back(obj);
    weights_.push_back(weight);

    weightSum_ += weight;

    //bsASSERT(weightSum_ <= base::threshold_, "threshold_ greater than sum of weights");

    base::extbounds_.append ( obj->bounds() );

    bs_aabb& bounds = base::extbounds_;
    bs_aabb* result = &bounds;

    return result;
}

template<typename T>
T* bs_quadtree<T>::find(float x, float y)
{
    bs_vector3f objcoord(x,1.f,y);
    node* curn = rootn;
    bool done=false;
    int i=0;
    while (!done)
    {
        for (i=0;i<4;i++)
        {
            if (curn->children_[i]->bounds_.inside_xz(objcoord))
            {
                if (!curn->children_[i]->isLeaf_)
                {
                    curn = dynamic_cast<node*>(curn->children_[i]);
                    i = 4;
                }
                else
                {
                    leaf* curleaf = (leaf*) curn->children_[i];
                    bs_vector<T*> items = curleaf->objs_;
                    for (size_t k = 0; k < items.size(); k++)
                    {
                        if ((items[k])->waabb().inside_xz(objcoord))
                        {
                            done = true;
                            return (items[k]);
                        }
                    }
                    return(0);
                }
            }
        }
        if (i == 4)
            done = true;
    }
    return(0);
}

template<typename T>
typename bs_quadtree<T>::leaf* bs_quadtree<T>::leaf_iterator::iterate(base* child)
{
    while (child->parent_)
    {
        size_t i = 0;
        // find childIdx
        while (child->parent_->children_[i] != child)
            ++i;

        if (i == 3)
            child = child->parent_;
        else {
            ++i;
            child = child->parent_->children_[i];

            while (!child->isLeaf_)
                child = ((node*)child)->children_[0];

            return (leaf*) child;
        }
    }

    // OK, finished
    return NULL;
}

template<typename T>
typename bs_quadtree<T>::base* bs_quadtree<T>::iterator::iterate(base* child)
{
    if (child->isLeaf_ || child->finishedChildren_ == 4)
    {
        if (child->isLeaf_)
            child = child->parent_;

        while (child->finishedChildren_ == 4)
        {
            child->finishedChildren_ = 0;
            if (child == root_)
                return NULL;
            child = child->parent_;
        }
    }

    return ((node*)child)->children_[child->finishedChildren_++];
}

template<typename T>
void bs_quadtree<T>::iterator::skip() {

    node_->finishedChildren_ = 0;

    if (node_->parent_)
        node_ = iterate(node_->parent_);

    else
        node_ = NULL;
}


#endif
