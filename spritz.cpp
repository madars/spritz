#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>

using namespace std;

void calculate_spritz(const size_t N, const size_t num_tries, const size_t num_outputs)
{
    std::vector<float> chis;

    assert(N * N * N <= 5 * num_outputs);

    double expected = (1.*num_outputs)/N/N/N;
    double expected_chi = (N-1)*(N-1)*(N-1);
    double expected_stddev_chi = sqrt(2.*expected_chi);

    for (size_t trial = 0; trial < num_tries; ++trial)
    {
#ifdef DEBUG
        printf("running trial %zu out of %zu\n", trial+1, num_tries);
#else
        if (trial % 1000 == 999)
        {
            printf("running trial %zu out of %zu\n", trial+1, num_tries);
        }
#endif

        std::vector<std::vector<std::vector<size_t> > > count_table;

        count_table.resize(N);
        for (size_t i = 0; i < N; ++i)
        {
            count_table[i].resize(N);
            for (size_t j = 0; j < N; ++j)
            {
                count_table[i][j].resize(N);
            }
        }

        const size_t num_prevs = 10;

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
            z = S[(j + S[(i + S[(z + k) % N]) % N]) % N];

            buf_i[buf_pos] = i;
            buf_j[buf_pos] = j;
            buf_k[buf_pos] = k;
            buf_z[buf_pos] = z;
            buf_pos = (buf_pos == num_prevs-1 ? 0 : buf_pos + 1);

            const size_t idx1 = i;
            const size_t idx2 = buf_z[(buf_pos+num_prevs-3) % num_prevs];
            const size_t idx3 = buf_z[(buf_pos+num_prevs-1) % num_prevs];

            count_table[idx1][idx2][idx3] += 1;
        }

        double chi = 0;
        for (size_t i = 0; i < N; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                for (size_t k = 0; k < N; ++k)
                {
                    chi += (count_table[i][j][k] - expected)*(count_table[i][j][k] - expected);
                }
            }
        }
        chi /= expected;
        chis.emplace_back(chi);
#ifdef DEBUG
        printf("got chi value of %0.3f (expected: %0.3f, deviations from expected: %0.3f)\n", chi, expected_chi, fabs((chi-expected_chi)/expected_stddev_chi));
#endif
    }

    double chi_avg = 0;
    for (size_t i = 0; i < num_tries; ++i)
    {
        chi_avg += chis[i] / num_tries;
    }

    double chi_stddev = 0;
    for (size_t i = 0; i < num_tries; ++i)
    {
        chi_stddev += (chis[i] - chi_avg) * (chis[i] - chi_avg) / num_tries;
    }
    chi_stddev = sqrt(chi_stddev);

    printf("average chi squared: %0.3f\n", chi_avg);
    printf("stddev(chi squared): %0.3f\n", chi_stddev);
    printf("expected chi squared: %0.3f\n", expected_chi);
    printf("expected standard deviation: %0.3f\n", expected_stddev_chi);
    printf("average is x standard deviations away from average for x = %0.3f\n", fabs((chi_avg-expected_chi)/expected_stddev_chi));
}

int main(void)
{
    calculate_spritz(16, 1ul<<18, 1ul<<18);
}
