import * as React from 'react'
import {
  SafeAreaView,
  StatusBar,
  StyleSheet,
  Text,
  View
} from 'react-native'

import { tests } from './tests'

interface Results {
  [name: string]: undefined | true | string
}

export function FastCryptoTest(): JSX.Element {
  const [results, setResults] = React.useState<Results>({})

  async function runAllTests(): Promise<void> {
    console.log(`Running ${Object.keys(tests).length} crypto tests...`)

    for (const name in tests) {
      try {
        console.log(`Running: ${name}`)
        await tests[name]()
        console.log(`PASSED: ${name}`)
        setResults(results => ({ ...results, [name]: true }))
      } catch (e: any) {
        console.error(`FAILED: ${name} - ${String(e)}`)
        setResults(results => ({ ...results, [name]: String(e) }))
      }
    }

    console.log('Tests completed')
  }

  React.useEffect(() => {
    console.log('FastCryptoTest - Starting tests...')
    setTimeout(() => runAllTests(), 1000)
  }, [])

  function statusLine(
    name: string,
    status: undefined | true | string
  ): JSX.Element {
    if (status == null) {
      return (
        <Text key={name} style={styles.running}>
          Running "{name}"
        </Text>
      )
    }
    if (status === true) {
      return (
        <Text key={name} style={styles.good}>
          Passed "{name}"
        </Text>
      )
    }
    return (
      <Text key={name} style={styles.bad}>
        Failed "{name}" ({status})
      </Text>
    )
  }

  return (
    <SafeAreaView style={styles.container}>
      <StatusBar
        barStyle="light-content"
        translucent
        backgroundColor="#20503000"
      />
      <Text style={styles.header}>Fast Crypto Tests</Text>
      <View style={styles.results}>
        {Object.keys(tests).map(name => statusLine(name, results[name]))}
      </View>
    </SafeAreaView>
  )
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#205030',
    padding: 20
  },
  header: {
    fontSize: 24,
    fontWeight: 'bold',
    color: '#ffffff',
    textAlign: 'center',
    marginBottom: 20
  },
  results: {
    flex: 1,
    backgroundColor: '#ffffff',
    borderRadius: 8,
    padding: 10
  },
  running: {
    color: '#888888',
    fontSize: 14,
    marginBottom: 3,
    fontFamily: 'monospace'
  },
  good: {
    color: '#008800',
    fontSize: 14,
    marginBottom: 3,
    fontFamily: 'monospace'
  },
  bad: {
    color: '#cc0000',
    fontSize: 14,
    marginBottom: 3,
    fontFamily: 'monospace'
  }
})