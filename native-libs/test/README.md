### New files to be copied from upstream

- `epee/include/storages`
- `epee/include/file_io_utils.h`

### Data needed for processing

- `txId`
- `pub` - `rawTx.prefix.extra.transaction_public_key`
- `version`
- `rv`
- `outputs`

### Needed to be returned for discovered UTXOs

- `txId`
- `vout`
- `amount`
- `keyImage`
- `txPub`
- `pub`
- `rct`
- `globalIndex`
- `blockHeight`

### Needed to be returned for all txs

### Output format

Note: Only add found outputs. Only add inputs that match globalKeyImages. Skip transactions with 0 ins and outs.

```json
{
  "current_height": "21",
  "txs": [
    {
      "id": "transaction id",
      "timestamp": "timestamp",
      "height": "block height",
      "pub": "transaction pub",
      "pid": "payment id",
      "epid": "encrypted payment id",
      "fee": "transaction fee",
      "inputs": ["key image of the input"],
      "utxos": [
        {
          "vout": "vout number",
          "amount": "amount of output",
          "key_image": "key image of the output",
          "pub": "output public key",
          "rv": "output rct",
          "global_index": "output global index"
        }
      ]
    }
  ]
}
```
