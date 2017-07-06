
Pod::Spec.new do |s|
  s.name         = "RNFastCrypto"
  s.version      = "0.0.1"
  s.summary      = "RNFastCrypto"
  s.description  = <<-DESC
                  RNFastCrypto
                   DESC
  s.homepage     = "https://airbitz.co"
  s.license      = "MIT"
  # s.license      = { :type => "MIT", :file => "FILE_LICENSE" }
  s.author             = { "Paul Puey" => "paul@airbitz.co" }
  s.platform     = :ios, "7.0"
  s.source       = { :git => "https://github.com/Airbitz/react-native-fast-crypto.git", :tag => "master" }
  s.source_files  = "RNFastCrypto/**/*.{h,m}"
  s.requires_arc = true


  s.dependency "React"
  #s.dependency "others"

end

  