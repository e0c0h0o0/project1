#ifndef CS660_SEQSCAN_H
#define CS660_SEQSCAN_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <db/HeapFile.h>




namespace db {
    class SeqScanIterator {
        const void *seqScan;
        HeapFileIterator heapFileIterator;

    public:
        SeqScanIterator(const void *ss, HeapFileIterator it);
        bool operator!=(const SeqScanIterator &other) const;

        SeqScanIterator &operator++();

        const Tuple &operator*() const;
    };

    /**
     * SeqScan is an implementation of a sequential scan access method that reads
     * each tuple of a table in no particular order (e.g., as they are laid out on
     * disk).
     */
    class SeqScan {
        using iterator = SeqScanIterator;
        friend class SeqScanIterator;
        TransactionId *transactionId;
        int tableid;
        std::string tableAlias;
        DbFile *dbFile;

    public:


        /**
         * Creates a sequential scan over the specified table as a part of the
         * specified transaction.
         *
         * @param tid
         *            The transaction this scan is running as a part of.
         * @param tableid
         *            the table to scan.
         * @param tableAlias
         *            the alias of this table (needed by the parser); the returned
         *            tupleDesc should have fields with name tableAlias.fieldName
         *            (note: this class is not responsible for handling a case where
         *            tableAlias or fieldName are null. It shouldn't crash if they
         *            are, but the resulting name can be null.fieldName,
         *            tableAlias.null, or null.null).
         */
        SeqScan(TransactionId *tid, int tableid, const std::string &tableAlias);

        /**
         * @return
         *       return the table name of the table the operator scans. This should
         *       be the actual name of the table in the catalog of the database
         * */
        std::string getTableName() const;

        /**
         * @return Return the alias of the table this operator scans.
         * */
        std::string getAlias() const;

        /**
         * Reset the tableid, and tableAlias of this operator.
         * @param tableid
         *            the table to scan.
         * @param tableAlias
         *            the alias of this table (needed by the parser); the returned
         *            tupleDesc should have fields with name tableAlias.fieldName
         *            (note: this class is not responsible for handling a case where
         *            tableAlias or fieldName are null. It shouldn't crash if they
         *            are, but the resulting name can be null.fieldName,
         *            tableAlias.null, or null.null).
         */
        void reset(int tableid, const std::string &tableAlias);

        SeqScan(TransactionId *tid, int tableid) :
                SeqScan(tid, tableid, Database::getCatalog().getTableName(tableid)) {}

        /**
         * Returns the TupleDesc with field names from the underlying HeapFile,
         * prefixed with the tableAlias string from the constructor. This prefix
         * becomes useful when joining tables containing a field(s) with the same
         * name.  The alias and name should be separated with a "." character
         * (e.g., "alias.fieldName").
         *
         * @return the TupleDesc with field names from the underlying HeapFile,
         *         prefixed with the tableAlias string from the constructor.
         */
        const TupleDesc &getTupleDesc() const;

        iterator begin() const;

        iterator end() const;
    };
}
#endif
