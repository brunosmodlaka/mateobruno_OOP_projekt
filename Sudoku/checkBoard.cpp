#include "checkBoard.h"


namespace check {

    static void markDuplicatesIn9(const int idxs[9], const puzzle::Puzzle& b, ErrorMap& err)
    {
        int count[10] = { 0 }; 

        for (int i = 0; i < 9; i++) {
            std::uint8_t v = b[idxs[i]];
            if (v >= 1 && v <= 9) count[v]++;
        }

        for (int i = 0; i < 9; i++) {
            std::uint8_t v = b[idxs[i]];
            if (v >= 1 && v <= 9 && count[v] > 1) {
                err[idxs[i]] = true;
            }
        }
    }
    int provjeranule(puzzle::Puzzle& b) {
        for (int i = 0; i < 81; i++) {
        if(b[i]==0){
            return 0;;
        }
        

       }
        return 1;
        
 }
    

    ErrorMap computeErrors(const puzzle::Puzzle& board)
    {
        ErrorMap err{};
        err.fill(false);

        // row
        for (int r = 0; r < 9; r++) {
            int idxs[9];
            for (int c = 0; c < 9; c++) idxs[c] = r * 9 + c;
            markDuplicatesIn9(idxs, board, err);
        }

        // cols
        for (int c = 0; c < 9; c++) {
            int idxs[9];
            for (int r = 0; r < 9; r++) idxs[r] = r * 9 + c;
            markDuplicatesIn9(idxs, board, err);
        }
        //zero

        

        //3x3
        
        for (int br = 0; br < 3; br++) {
            for (int bc = 0; bc < 3; bc++) {
                int idxs[9];
                int k = 0;
                for (int r = br * 3; r < br * 3 + 3; r++) {
                    for (int c = bc * 3; c < bc * 3 + 3; c++) {
                        idxs[k++] = r * 9 + c;
                    }
                }
                markDuplicatesIn9(idxs, board, err);
            }
        }

        return err;
    }

    bool anyError(const ErrorMap& e)
    {
        for (bool b : e)
            if (b) return true;
        return false;
    }

} // namespace check
