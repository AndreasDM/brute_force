#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <future>
#include <vector>

constexpr char alph[] = "abcdefghijklmnopqrstuvwxyz";
int alph_map[256];

std::size_t search_section(int length, int end_index) noexcept
{
    std::size_t combinations{};
    char s[length+1];
    memset(s, alph[0], sizeof(s)-1);
    s[length] = 0;

    for (;;)
    {
        // puts(s);
        ++combinations;

        for (int i = length - 1; i >= 0; --i)
        {
            const int c = alph_map[s[i]];

            s[i] = (alph_map[s[i]] == end_index) ? (alph[0]) : (alph[alph_map[s[i]] + 1]);

            if (c == end_index && i == 0) return combinations;
            else if (c != end_index)      break;
        }
    }
}

long long search_section_2(int length, int end_index, int start_at, int stop_at) noexcept
{
    long long combinations{};
    char s[length+1];
    memset(s, alph[0], sizeof(s)-1);
    s[0] = alph[start_at];
    s[length] = 0;

    while (1)
    {
        // puts(s);
        ++combinations;

        for (int i = length - 1; i >= 0; --i)
        {
            const int c = alph_map[s[i]];

            s[i] = (alph_map[s[i]] == end_index) ? (alph[0]) : (alph[alph_map[s[i]] + 1]);

            if (i == 0 && (c == end_index || c == stop_at)) return combinations;
            else if (c != end_index)                        break;
            else ;
        }
    }
}

int main(int argc, char * argv[])
{
    constexpr int length = 7;

    for (int i = 0; i < sizeof(alph)-1; ++i)
        alph_map[alph[i]] = i;

    constexpr int e = (sizeof(alph)-2); // End alphabet index
    constexpr int m = e/2;              // Middle index
    constexpr int mm = m/2;
    constexpr int mme = (m + e)/2;
    constexpr int fast_for_one_thread = 5;

    long long combinations {};

    std::vector<std::future<long long>> futures;

    const auto run = [&]()
    {
        for (int i = 1; i <= length && i <= fast_for_one_thread; ++i)
        {
            combinations += search_section(i, e);
        }

        for (int i = fast_for_one_thread + 1; i <= length; ++i)
        {
            futures.emplace_back(std::async(std::launch::async, search_section_2, i, e, 0, mm));
            futures.emplace_back(std::async(std::launch::async, search_section_2, i, e, mm+1, m));
            futures.emplace_back(std::async(std::launch::async, search_section_2, i, e, m+1, mme));
            futures.emplace_back(std::async(std::launch::async, search_section_2, i, e, mme+1, e));
        }

        for (auto & f : futures)
            combinations += f.get();
    };

    auto start = std::chrono::system_clock::now();
    run();
    auto end = std::chrono::system_clock::now();

    auto res = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("Took %ld ms\n", res);
    printf("Combinations: %lld\n", combinations);
}
