#include <bits/stdc++.h>
#include <string.h>
#include <iostream>
using namespace std;

string ones_complement(string data)
{
    for (size_t i = 0; i < data.length(); i++)
    {
        if (data[i] == '0')
            data[i] = '1';
        else
            data[i] = '0';
    }

    return data;
}

string calcCheckSum(string data, int block_size = 8)
{
    int n = data.length();
    if (n % block_size != 0)
    {
        int pad_size = block_size - (n % block_size);
        for (int i = 0; i < pad_size; i++)
        {
            data = '0' + data;
        }
    }

    string result = "";

    for (int i = 0; i < block_size; i++)
    {
        result += data[i];
    }

    for (int i = block_size; i < n; i += block_size)
    {
        string next_block = "";

        for (int j = i; j < i + block_size; j++)
        {
            next_block += data[j];
        }

        string additions = "";
        int sum = 0, carry = 0;

        for (int k = block_size - 1; k >= 0; k--)
        {
            sum += (next_block[k] - '0') + (result[k] - '0');
            carry = sum / 2;
            if (sum == 0)
            {
                additions = '0' + additions;
                sum = carry;
            }
            else if (sum == 1)
            {
                additions = '1' + additions;
                sum = carry;
            }
            else if (sum == 2)
            {
                additions = '0' + additions;
                sum = carry;
            }
            else
            {
                additions = '1' + additions;
                sum = carry;
            }
        }

        string final = "";

        if (carry == 1)
        {
            for (int l = additions.length() - 1; l >= 0;
                 l--)
            {
                if (carry == 0)
                {
                    final = additions[l] + final;
                }
                else if (((additions[l] - '0') + carry) % 2 == 0)
                {
                    final = "0" + final;
                    carry = 1;
                }
                else
                {
                    final = "1" + final;
                    carry = 0;
                }
            }

            result = final;
        }
        else
            result = additions;
    }
    return ones_complement(result);
}

bool checker(string rec_message, string rec_checksum, int block_size = 8)
{
    string msg_checksum = calcCheckSum(rec_message, block_size);

    if (count(msg_checksum.begin(), rec_checksum.end(), '0') == block_size)
        return true;
    else
        return false;
}

