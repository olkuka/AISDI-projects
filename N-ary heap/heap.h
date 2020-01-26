#include <utility>
#include <iostream>
#include <vector>
#include <climits>

#define H 3

template<typename KeyType, typename ValueType>
class Heap3
{
  private:
    using KeyValueType = std::pair<KeyType, ValueType>;
	std::vector<KeyValueType> heap;

	void heapifyDown() {
        int index = 0;
        while(index * H < heap.size() - 1) {
            int maxIndex = index * H + 1;
            for(int i = index * H + 2; i < (heap.size() - 1) && (i <= index * H + H); i++) {
                if(heap[maxIndex].first > heap[i].first)
                    maxIndex = i;
            }
            if(heap[index].first > heap[maxIndex].first)
                std::swap(heap[index], heap[maxIndex]);
            else break;

            index = maxIndex;
        }
    }

	inline int parent(int index) const {
			return (index - 1) / H;
	}

	template<typename StreamType>
	void print(StreamType &stream, size_t index, int tab) const {
        if(index < heap.size()) {
            for(size_t i = 0; i < tab; i++)
                stream << "\t";

            stream << heap[index].first << std::endl;
            for(size_t i = 0; i < H; i++) 
                print(stream, child(index)+i, tab+1);
        }
    }

	int child(size_t index) const {
        size_t ch = H * index + 1;

		return (ch > heap.size() -1 ? INT_MAX : ch);
			// jesli dziecko nie istnieje to zwracam INT_MAX, czyli najwyzsza mozliwa wartosc int
	}

public:

	bool empty() const noexcept
	{
		return heap.size() == 0;
	}

	void insert(KeyType const &key, ValueType const &value)
	{
		insert(std::make_pair(key, value));
	}

	void insert(KeyValueType const &key_value)
	{
		heap.push_back(key_value);
        int i = heap.size() - 1;
        int j = parent(i) ;      // zapisujemy numer wezla z nowym elementem i szukamy numeru wezla rodzica

        while ( j >= 0 && i != j && heap[j].first > heap[i].first ) {
            std::swap(heap[i], heap[j]);
            i = j;
            j = parent(i) ;      // znajdujemy numer wezla rodzica
        }
	}

	KeyValueType const & peek() const
	{
		if (!empty())
            return heap.front();
		else
            throw std::logic_error("The heap is empty.");
	}

	KeyValueType pop()
	{
		if(!empty()) {
            KeyValueType r = heap[0];
            std::swap(heap[0], heap[heap.size()-1]);
            heap.pop_back();
			if( heap.size()>1 ) heapifyDown();
            return r;
        } 
        else
            throw std::logic_error("The heap is empty!");

	}

	size_t size() const noexcept
	{
		return heap.size();
	}

	template<typename StreamType>
	void print(StreamType &stream) const
	{
		print(stream, 0, 0);
		stream << std::endl;
	}

};
