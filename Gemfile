source 'https://rubygems.org'

gem 'bugsnag-maze-runner', '~>9.0'
gem 'cocoapods'

# Only install bumpsnag if we're using Github actions
unless ENV['GITHUB_ACTIONS'].nil?
  gem 'bumpsnag', git: 'https://github.com/bugsnag/platforms-bumpsnag', branch: 'main'
end
