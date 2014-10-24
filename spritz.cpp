#include <algorithm>
#include <random>

using namespace std;

void calculate_spritz(const size_t N, const size_t num_outputs)
{
    std::vector<std::vector<size_t> > bias_table(N, std::vector<size_t>(N, 0));

    const size_t num_prevs = 3;

    std::random_device rd;
    unsigned char S[N];
    std::iota(S, S+N, 0);
    std::shuffle(S, S+N, rd);

    size_t i = 0, j = 0, k = 0, z;
    size_t buf_i[num_prevs] = {0};
    size_t buf_j[num_prevs] = {0};
    size_t buf_k[num_prevs] = {0};
    size_t buf_z[num_prevs] = {0};

    size_t buf_pos =  0;
    for (size_t outno = 0; outno < num_outputs; ++outno)
    {
        i = (i + 1) % N;
        j = (k + S[(j + S[i]) % N]) % N;
        k = (i + k + S[j]) % N;
        std::swap(S[i], S[j]);
        z = S[(j + S[(i + S[(k + z) % N]) % N]) % N];

        buf_i[buf_pos] = i;
        buf_j[buf_pos] = j;
        buf_k[buf_pos] = k;
        buf_z[buf_pos] = z;

        bias_table[buf_z[(buf_pos+num_prevs-3) % num_prevs]][z] += 1;

        buf_pos = (buf_pos == num_prevs-1 ? 0 : buf_pos + 1);
    }
}

int main(void)
{
    calculate_spritz(16, 1<<24);
}
