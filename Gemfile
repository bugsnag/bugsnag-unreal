source 'https://rubygems.org'

# Only install bumpsnag if we're using Github actions
if ENV['GITHUB_ACTIONS'].nil?
  gem 'bugsnag-maze-runner', '~>11.2'
  gem 'cocoapods'
else
  gem 'bumpsnag', git: 'https://github.com/bugsnag/platforms-bumpsnag', branch: 'main'
end
